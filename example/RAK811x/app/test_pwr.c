/******************************************************************************
* @file     main.c
* @brief    The sample code. User can add to main() to build first project
* @version  1.0.1
* @date     04, September, 2012
*
* @note
* Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
******************************************************************************/
#include <stdio.h>
#include "nano1xx_sys.h"
#include "nano1xx_rtc.h"
#include "nano1xx_gpio.h"
#include "nano1xx_timer.h"


#define TIMER0_WAKEUP_TEST 0
#define RTC_WAKEUP_TEST    1
#define WDT_WAKEUP_TEST    0
#define GPIOA_WAKEUP_TEST  0
#define NORMAL_RUN_TEST    0  //0-powerdown mode    1-normal mode

uint8_t led_state;
/*---------------------------------------------------------------------------------------------------------*/
/* RTC Tick Handle                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void RTC_TickHandle(void)
{
    S_DRVRTC_TIME_DATA_T sCurTime;
    
    /* Get the currnet time */
    RTC_Read(&sCurTime);
    
    printf("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);				
    if(led_state==0){
        led_state = 1;
        GPIO_SetBit(GPIOE, 6);
    }else{
        led_state = 0;
        GPIO_ClrBit(GPIOE, 6);
    }
    
    
}

/* Global variables */
__IO int32_t   _Wakeup_Flag = 0;	/* 1 indicates system wake up from power down mode */
__IO uint32_t  _Pin_Setting[11];	/* store Px_H_MFP and Px_L_MFP */
__IO uint32_t  _PullUp_Setting[6];	/* store GPIOx_PUEN */



/**
* @brief  Store original setting of multi-function pin selection. 
* @param  None.
* @return None.
*/
void SavePinSetting()
{
    /* Save Pin selection setting */
    _Pin_Setting[0] = GCR->PA_L_MFP;
    _Pin_Setting[1] = GCR->PA_H_MFP;
    _Pin_Setting[2] = GCR->PB_L_MFP;
    _Pin_Setting[3] = GCR->PB_H_MFP;
    _Pin_Setting[4] = GCR->PC_L_MFP;
    _Pin_Setting[5] = GCR->PC_H_MFP;
    _Pin_Setting[6] = GCR->PD_L_MFP;
    _Pin_Setting[7] = GCR->PD_H_MFP;
    _Pin_Setting[8] = GCR->PE_L_MFP;
    _Pin_Setting[9] = GCR->PE_H_MFP;
    _Pin_Setting[10] = GCR->PF_L_MFP;
    
    /* Save Pull-up setting */
    _PullUp_Setting[0] =  GPIOA->PUEN;
    _PullUp_Setting[1] =  GPIOB->PUEN;
    _PullUp_Setting[2] =  GPIOC->PUEN;
    _PullUp_Setting[3] =  GPIOD->PUEN;
    _PullUp_Setting[4] =  GPIOE->PUEN;
    _PullUp_Setting[5] =  GPIOF->PUEN;
}


/**
* @brief  Restore original setting of multi-function pin selection. 
* @param  None.
* @return None.
*/
void RestorePinSetting()
{
    /* Restore Pin selection setting */
    GCR->PA_L_MFP = _Pin_Setting[0];
    GCR->PA_H_MFP = _Pin_Setting[1];
    GCR->PB_L_MFP = _Pin_Setting[2];
    GCR->PB_H_MFP = _Pin_Setting[3];
    GCR->PC_L_MFP = _Pin_Setting[4];
    GCR->PC_H_MFP = _Pin_Setting[5];
    GCR->PD_L_MFP = _Pin_Setting[6];
    GCR->PD_H_MFP = _Pin_Setting[7];
    GCR->PE_L_MFP = _Pin_Setting[8];
    GCR->PE_H_MFP = _Pin_Setting[9];
    GCR->PF_L_MFP = _Pin_Setting[10];
    
    /* Rstore Pull-up setting */
    GPIOA->PUEN = _PullUp_Setting[0];
    GPIOB->PUEN = _PullUp_Setting[1];
    GPIOC->PUEN = _PullUp_Setting[2];
    GPIOD->PUEN = _PullUp_Setting[3];
    GPIOE->PUEN = _PullUp_Setting[4];
    GPIOF->PUEN = _PullUp_Setting[5];
}


/**
* @brief  Save multi-functon pin setting and then go to power down. 
* @param  None.
* @return None.
*/
void initGPIO()
{
    /* Back up original setting */
    SavePinSetting();
    
    /* Set function pin to GPIO mode */
    GCR->PA_L_MFP = 0;
    GCR->PA_H_MFP = 0;
    GCR->PB_L_MFP = 0;
    GCR->PB_H_MFP = 0;
    GCR->PC_L_MFP = 0;
    GCR->PC_H_MFP = 0;
    GCR->PD_L_MFP = 0;
    GCR->PD_H_MFP = 0;
    GCR->PE_L_MFP = 0;
    GCR->PE_H_MFP = 0;
    GCR->PF_L_MFP = 0x00007700;
    
    /* Enable GPIO pull up */
    GPIOA->PUEN = 0xFFFF;
    GPIOB->PUEN = 0xFFFF;
    GPIOC->PUEN = 0xFFFF;
    GPIOD->PUEN = 0xFFFF;
    GPIOE->PUEN = 0xFFFF;
    GPIOF->PUEN = 0x0033;	  /* exclude GPF2 and GPF3 which are HXT OUT/IN */
    
    CLK->AHBCLK |= CLK_AHBCLK_GPIO_EN;  
    /* Disable LCD clock */
    // SYS_SetChipClockSrc(CLK_PWRCTL_LIRC_EN, 0); /* disable LXT - 32Khz */
    // SYS_SetChipClockSrc(CLK_PWRCTL_LXT_EN, 0); /* disable LXT - 32Khz */
    
    
}


/**
* @brief  This function will not be called.
* @param  None.
* @return None.
*/
void Leave_PowerDown()
{
    
    /* Restore pin setting */
    RestorePinSetting();
    
    /* Set PF.0 and PF.1 to ICE Data and Clock */
    GCR->PF_L_MFP |= 0x00000077;	 	
    
    /* Enable LCD clock */
    SYS_SetIPClock_2(CLK_APBCLK_LCD_EN, 1);
}




int32_t main (void)
{
    uint32_t g_u32TICK;
    S_DRVRTC_TIME_DATA_T sInitTime;
    S_SYS_CHIP_CLKCFG sChipClkCfg;
    
    
    
    UNLOCKREG(); // ¨¬?¡Àe¡Á¡é¨°a¡ê?¨°???¨º¨¹¡À¡ê?¡è¦Ì???¡ä??¡Â2??¨¹?a??
    
#if(RTC_WAKEUP_TEST==1)
    CLK->PWRCTL |=  CLK_PWRCTL_LXT_EN;
    while(SYS_CheckChipClockSrc(CLK_CLKSTATUS_LXT_STB) != 0) ;
#endif
    
    ///CLK->CLKSEL0=0x07; //select HIRC for HCLK;
    ///CLK->CLKDIV0=0x02; //HCLK/(2+1) =12M/3=4M;
    
    
    initGPIO();
    GPIOD->PMD = 1;  //PD0 set output
    GPIOD->PUEN = 0xFFFE;
    
    RTC_Init();
    // Time Setting 
    sInitTime.u32Year 		= 2012;
    sInitTime.u32cMonth 	= 7;
    sInitTime.u32cDay 		= 1;
    sInitTime.u32cHour 		= 12;
    sInitTime.u32cMinute 	= 30;
    sInitTime.u32cSecond 	= 0;
    sInitTime.u32cDayOfWeek = DRVRTC_TUESDAY;
    sInitTime.u8cClockDisplay = DRVRTC_CLOCK_24;
    sInitTime.u8IsEnableWakeUp = 0;
    
    // Initialization the RTC timer 
    if(RTC_Open(&sInitTime) !=E_SUCCESS)
    {
        //printf("RTC Open Fail!!\n");
        //return false;
    }		
    //RTC_SetTickMode(DRVRTC_TICK_1_SEC);
    RTC->TTR |=BIT3;// PowerDown wakeup on
    // Enable RTC Tick Interrupt 
    RTC_EnableInt(RTC_RIER_TIER);	
    
    
    
    while(1)
    {  
        int i;
        for(i = 0; i < 1000000; i++) {
            __NOP();
        }
        SYS_SetUpPowerDown(1);	/* Disable PDWU interrupt */
        __WFI();	 /* system really enter power down here ! */
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
    }
}


#ifdef USE_ASSERT
/**
* @brief  The function prints the source file name and line number where the assert_param() error
*         occurs, and then stops in an infinite loop. User can add his own codes here if necessary.
* @param[in] file Source file name
* @param[in] line Line number
* @return None
*/
void assert_error(uint8_t * file, uint32_t line)
{
    GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0x77) | (PB1_MFP_UART0_TX | PB0_MFP_UART0_RX);  /* Select multi-function pin for UART0 */
    CLK->APBCLK |= CLK_APBCLK_UART0_EN;    /* Enable UART0 clock */
    CLK->CLKDIV0 &= ~CLK_CLKDIV0_UART_MASK;
    CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HXT;  /* Select 12 Mhz XTAL */
    
    
    /* Set UART to 115200,n,8,1,none */
    UART0->BAUD = 0x67;             /* Baud Rate:115200 for 12MHz */
    UART0->TLCTL = 0x03;            /* Word len is 8 bits */          
    
    printf("[%s] line %d : wrong parameters.\r\n", file, line);
    
    /* Infinite loop */
    while(1) ;
    
}
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
