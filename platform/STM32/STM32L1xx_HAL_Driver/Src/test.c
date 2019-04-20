/*
/ _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
\____ \| ___ |    (_   _) ___ |/ ___)  _ \
_____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
(C)2013 Semtech

Description: LoRaMac classA device implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/

/*! \file classA/LoRaMote/main.c */
#include <time.h>
#include <string.h>
#include "board.h"


#include "app.h"

#include "rw_lora.h"

/** @addtogroup STM32L1xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_STOP_RTC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void SystemClockConfig_STOP(void);
static void Error_Handler(void);

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Configue the RTC clock soucre ######################################*/
  /* -a- Enable LSI Oscillator */
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }

  /* -b- Select LSI as RTC clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    while(1);
  }

  /*##-2- Enable the RTC peripheral Clock ####################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
  
  /*##-3- Configure the NVIC for RTC Alarm ###################################*/
  HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

/**
  * @brief RTC MSP De-Initialization 
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();
}


void RTC_WKUP_IRQHandler( void )
{
  HAL_RTCEx_WakeUpTimerIRQHandler(&RTCHandle);

}

int main(void)
{
    uart_config_t uart_config;
  /* Configure the system clock to 32 MHz */
    BoardInitMcu();
  
  
    if (read_partition(PARTITION_1, (char *)&uart_config, sizeof(uart_config)) < 0) {
        SET_UART_CONFIG_DEFAULT(uart_config);
    } 
    SET_UART_CONFIG_DEFAULT(uart_config);
    UartMcuInit(&Uart1, 1, PA_9, PA_10);
    UartMcuConfig(&Uart1, RX_TX, uart_config.baudrate, 
                                      uart_config.wordLength,
                                      uart_config.stopBits,
                                      uart_config.parity,
                                      uart_config.flowCtrl);
    //TimerIdleInit();
    //rw_InitLoRaWAN();

    e_printf("Welcome to RAK811\r\n");  

//  /* Configure RTC */
//  RTCHandle.Instance = RTC;
//#define RTC_ASYNCH_PREDIV    0x7F
//#define RTC_SYNCH_PREDIV     0x0130
//  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
//  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
//  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
//  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
//  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
//  RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
//  HAL_RTC_Init(&RTCHandle);


  while (1)
  {
      /* Insert 5 seconds delay */
      HAL_Delay(5000);
      e_printf("111\r\n");  
      /* Disable Wakeup Counter */
      //HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);
      
      /* ## Setting the Wake up time ############################################*/
      /*  RTC Wakeup Interrupt Generation:
      Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI))
      Wakeup Time = Wakeup Time Base * WakeUpCounter 
      = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI)) * WakeUpCounter
      ==> WakeUpCounter = Wakeup Time / Wakeup Time Base
      
      To configure the wake up timer to 4s the WakeUpCounter is set to 0x1FFF:
      RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16 
      Wakeup Time Base = 16 /(~39.000KHz) = ~0,410 ms
      Wakeup Time = ~4s = 0,410ms  * WakeUpCounter
      ==> WakeUpCounter = ~4s/0,410ms = 9750 = 0x2616 */
      //HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 0x2616, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
      
      /* Enter Stop Mode */
      //HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
      InstallWakeUpPin();
      SysEnterUltraPowerStopMode();
      UninstallWakeUpPin();
      
      e_printf("222\r\n");  
      /* Configures system clock after wake-up from STOP: enable HSI, PLL and select
      PLL as system clock source (HSI and PLL are disabled automatically in STOP mode) */
      SystemClockConfig_STOP();
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLMUL                         = 6
  *            PLLDIV                         = 3
  *            Flash Latency(WS)              = 1
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable HSE Oscillator and Activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV          = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set Voltage scale1 as MCU will run at 32MHz */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief  System Power Configuration
  *         The system Power is configured as follow :
  *            + Regulator in LP mode
  *            + VREFINT OFF, with fast wakeup enabled
  *            + HSI as SysClk after Wake Up
  *            + No IWDG
  *            + Automatic Wakeup using RTC clocked by LSI (after ~4s)
  * @param  None
  * @retval None
  */
static void SystemPower_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  /* Enable Ultra low power mode */
  //HAL_PWREx_EnableUltraLowPower();

  /* Enable the fast wake up from Ultra low power mode */
  //HAL_PWREx_EnableFastWakeUp();

//  /* Enable GPIOs clock */
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  __HAL_RCC_GPIOC_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
//
//  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
//  GPIO_InitStructure.Pin = GPIO_PIN_All;
//  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStructure.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
//  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
//
//  /* Disable GPIOs clock */
//  __HAL_RCC_GPIOA_CLK_DISABLE();
//  __HAL_RCC_GPIOB_CLK_DISABLE();
//  __HAL_RCC_GPIOC_CLK_DISABLE();
//  __HAL_RCC_GPIOD_CLK_DISABLE();
//  __HAL_RCC_GPIOH_CLK_DISABLE();
 
  /* Configure RTC */
  RTCHandle.Instance = RTC;
  /* Configure RTC prescaler and RTC data registers as follow:
    - Hour Format = Format 24
    - Asynch Prediv = Value according to source clock
    - Synch Prediv = Value according to source clock
    - OutPut = Output Disable
    - OutPutPolarity = High Polarity
    - OutPutType = Open Drain */
  
  /* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the corresponding line to select the RTC Clock source */
  #define RTC_ASYNCH_PREDIV    0x7F
  #define RTC_SYNCH_PREDIV     0x0130
  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if(HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
}

/**
  * @brief  Configures system clock after wake-up from STOP: enable HSI, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
static void SystemClockConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};

//  /* Get the Oscillators configuration according to the internal RCC registers */
//  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
//
//  /* After wake-up from STOP reconfigure the system clock: Enable HSI and PLL */
//  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
//  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
//  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
//  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//  RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL6;
//  RCC_OscInitStruct.PLL.PLLDIV          = RCC_PLL_DIV3;
//  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn on the LED2 */
  //BSP_LED_On(LED2);
  while(1)
  {
  }
}

/**
  * @brief  RTC Wake Up callback
  * @param  None
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Clear Wake Up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
