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

lora_config_t g_lora_config;
TimerEvent_t ReadAdcHandleTimer;

#define ADC_AVERG_MAX     (10.0)

extern void lora_cli_loop(void);
extern TIM_HandleTypeDef    TimHandle;

static uint32_t appSend_TS = 0 ;
static uint32_t levelCheck_TS = 0 ;
static uint16_t adcBase = 0 ;

static float waterLevelm = 0 ;
static uint32_t  adcRead = 0 ;
static uint8_t   avgCnt  = 0 ;
static uint8_t   rfsDone = 0 ;

static uint8_t  appData[16];

#define APP_SEND_INTERVAL         (15*1000)
#define LEVEL_CHECK_INTERVAL      (5*1000)


#define ADC_LEVEL_BASE            (620) // 0.500V   0.5V - 2.5V  0-5 m


void test_cb1(void)
{
    e_printf("haha %d\r\n", RtcGetTimerValue());
}

 /*****************************************   readAdc_handle *********************************************************************/  
    //   0m    0.5v   ADC:620
    //   5m    2.5v   ADC:3103
    
    //   ?m    3.3v   ADC:4096      //实际不生效，因为要根据这个峰值计算对应的2.5V 对应的ADC值
    //   4096/3.3 *0.5V  = 620      对应的是 0.5V对应的ADC值
    //   4096/3.3 *2.5V  = 3103     对应的是 2.5V对应的ADC值
    //   如果采集到的值是1000,ADC。 那么对应的水深是：1000 * 3.3 /(4096) = 0.8v
    //   y(v)  =  0.4x(m) +0.5  
    //   x(m)  =  ((1000- 0 )*3.3/4096 -0.5)/0.4 = 2.1 = 210cm.
 /*******************************************************************************************************************************/  
void readAdc_handle(void)
{
    static float Adc_Aver=0;
    TimerStop( &ReadAdcHandleTimer );
    
    adcRead = AdcReadChannel( &WaterLevel_Adc, ADC_CHANNEL_18 );
   
    if (adcRead < ADC_LEVEL_BASE) {
       adcRead = ADC_LEVEL_BASE;
    }else if(adcRead>3103)adcRead=3103;
    
    Adc_Aver += adcRead;
    avgCnt++;
    
    if (avgCnt >= ADC_AVERG_MAX) {
      
        
       avgCnt  = 0 ;
       rfsDone = 1 ;
       Adc_Aver = Adc_Aver/ADC_AVERG_MAX;
       waterLevelm = (Adc_Aver-ADC_LEVEL_BASE)*5/(3103-ADC_LEVEL_BASE)*100.0;
       e_printf("Water Level :%.2f cm\r\n", waterLevelm); 
       
       Adc_Aver=0;
       

    }  else {    
        TimerSetValue( &ReadAdcHandleTimer, 20 );
        TimerStart( &ReadAdcHandleTimer );
    }  
}

void Start_WaterLevelCheck(void)
{
  rfsDone = 0 ;
  adcRead = 0 ;
  avgCnt  = 0 ;
  waterLevelm = 0;
  TimerSetValue( &ReadAdcHandleTimer, 20 );
  TimerStart( &ReadAdcHandleTimer );
}

void WaterLevel_app_loop(void)
{ 
    uint8_t   ret;
     
    if ( TimerGetCurrentTime() - levelCheck_TS >= LEVEL_CHECK_INTERVAL)
    {
        levelCheck_TS = TimerGetCurrentTime();
        Start_WaterLevelCheck( );
    }
     
    if ( (TimerGetCurrentTime() - appSend_TS >= APP_SEND_INTERVAL) && rfsDone )
    {  
        appSend_TS = TimerGetCurrentTime();

        appData[0] = 0x01;
        appData[1] = 0x03; 
        appData[2] = ((uint32_t)(waterLevelm*100) >> 8) & 0xFF;
        appData[3] = (uint32_t)(waterLevelm*100) & 0xFF; 
        
        if (g_lora_system.IsJoined == 0) {
          e_printf("Not joined\r\n");
          return;
        }
        
        ret = rw_LoRaTxData(0, 2, 4, appData);
        if (ret != LORAMAC_STATUS_OK) {
          e_printf("Tx Err: %d\r\n", ret);
        } else {
          e_printf("Tx OK: ");
          dump_hex2str(appData , 4);
        }
      
    } 
}


int main( void )
{
    uart_config_t uart_config;
    int ret;

    BoardInitMcu( );
    
    if (read_partition(PARTITION_1, (char *)&uart_config, sizeof(uart_config)) < 0) {
        SET_UART_CONFIG_DEFAULT(uart_config);
    } 
    
    UartMcuInit(&Uart1, 1, UART_TX, UART_RX);
    UartMcuConfig(&Uart1, RX_TX, uart_config.baudrate, 
                                      uart_config.wordLength,
                                      uart_config.stopBits,
                                      uart_config.parity,
                                      uart_config.flowCtrl);
									  
    e_printf("Welcome to RAK811_WaterLevel_DemoKit\r\n");
	
    BoardInitPeriph();
    
    rw_ReadUsrConfig();  
          
    rw_InitLoRaWAN();
    
    rw_LoadUsrConfig(); 
    
    
     //calibration
     if ( (adcBase =AdcReadChannel( &WaterLevel_Adc, ADC_CHANNEL_18 )) >= ADC_LEVEL_BASE)
     {
       e_printf("adcRead_RC :%.2f cm\r\n", adcBase); 
       adcBase = ADC_LEVEL_BASE;
     }
     e_printf("adcRead_BC :%.2f cm\r\n", adcBase); 
     e_printf("Water Level calibration: %.3f V\r\n",  (adcBase*3.3)/4096);

     if (g_lora_config.join_mode == ABP_MODE) 
     {
          ret = rw_JoinNetworkABP( g_lora_config.dev_addr, 
                                   g_lora_config.nwks_key, 
                                   g_lora_config.apps_key);
          if (ret == 0) 
          {
             e_printf("Join ABP...\r\n");
          } 
          else 
          {
             e_printf("No  parameters\r\n");
          }
    }
    else if(g_lora_config.join_mode == OTAA_MODE)
    {
      
         if (check_hex_invaild(g_lora_config.dev_eui, sizeof(g_lora_config.dev_eui)) != 0 ||
          check_hex_invaild(g_lora_config.app_key, sizeof(g_lora_config.app_key)) != 0 ||
          check_hex_invaild(g_lora_config.app_eui, sizeof(g_lora_config.app_eui)) != 0 ) 
         {
              e_printf("No OTAA parameters\r\n");
         } 
         else 
         {
              ret = rw_JoinNetworkOTAA(g_lora_config.dev_eui, 
                                       g_lora_config.app_eui, 
                                       g_lora_config.app_key, 
                                       g_lora_config.join_cnt);
              
              e_printf("Join OTAA...\r\n");      
         }
    

    }
    
    levelCheck_TS = appSend_TS = TimerGetCurrentTime();
    TimerInit( &ReadAdcHandleTimer, readAdc_handle);
    Start_WaterLevelCheck( );
    
    while(1) {
        lora_cli_loop();
        TimerLowPowerHandler( );
        WaterLevel_app_loop();
    }
}

