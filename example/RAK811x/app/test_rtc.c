#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_rtc.h"
#include "nano1xx_gpio.h"
#include "nano1xx_timer.h"


void TimerHwInit_( void )
{

    SYS_SelectIPClockSource_1(CLK_CLKSEL1_TMR0_MASK, CLK_CLKSEL1_TMR0_LIRC);
    //for(int i = 0; i < 50000; i++);
//    SYS_SelectIPClockSource_1(CLK_CLKSEL1_TMR1_MASK, CLK_CLKSEL1_TMR1_HIRC);
//    SYS_SelectIPClockSource_1(CLK_CLKSEL2_TMR2_MASK, CLK_CLKSEL2_TMR2_HIRC);


    /* deep_sleep timer */
    TIMER_Init(TIMER0, 250,  40*5, TIMER_CTL_MODESEL_PERIODIC);//32.768K/(11+1) * 1000 = 100ms
    //for(int i = 0; i < 50000; i++);
    TIMER_EnableInt(TIMER0, TIMER_IER_TMRIE);
    TIMER_EnableWakeUp(TIMER0);
    for(int i = 0; i < 50000; i++);
    TIMER_Start(TIMER0);
    //for(int i = 0; i < 50000; i++);
    
//    TIMER_Init(TIMER2, 11,  1000, TIMER_CTL_MODESEL_ONESHOT);//12M/(11+1) * 1000 = 1ms
//    TIMER_EnableInt(TIMER2, TIMER_IER_TMRIE);
//    //TIMER_DisableWakeUp(TIMER2);
//    TIMER_Start(TIMER2);
//
//    TIMER_Init(TIMER1, 11,  1000, TIMER_CTL_MODESEL_PERIODIC);//12M/(11+1) * 1000 = 1ms
//    TIMER_EnableInt(TIMER1, TIMER_IER_TMRIE);
//    //TIMER_DisableWakeUp(TIMER1);
//    TIMER_Start(TIMER1);

}

void RTC_IRQHandler(void)
{
    if ( (RTC->RIER & RTC_RIER_AIER) && (RTC->RIIR & RTC_RIIR_AIS) )		/* alarm interrupt occurred */
    {   
        RTC->RIIR = 0x1;
    }
    
    if ( (RTC->RIER & RTC_RIER_TIER))		/* alarm interrupt occurred */
    {   
        RTC->RIIR = RTC_RIER_TIER;
    } 
    
}
//
void TMR0_IRQHandler(void)
{

    TIMER_ClearIntFlag(TIMER0);
        //TIMER_Start(TIMER0);
}
//
//void TMR1_IRQHandler(void)
//{
//    TIMER_ClearIntFlag(TIMER1);
//}
//
//void TMR2_IRQHandler(void)
//{
//    TIMER_ClearIntFlag(TIMER2);
//    
//}

static void RtcSetConfig( void )
{
    S_DRVRTC_TIME_DATA_T sInitTime;
    /* RTC Initialize */
    RTC_Init();	
    
    /* Time Setting */
    sInitTime.u32Year 		= 2016;
    sInitTime.u32cMonth 	= 7;
    sInitTime.u32cDay 		= 27;
    sInitTime.u32cHour 		= 15;
    sInitTime.u32cMinute 	= 23;
    sInitTime.u32cSecond 	= 0;
    sInitTime.u32cDayOfWeek = DRVRTC_TUESDAY;
    sInitTime.u8cClockDisplay = DRVRTC_CLOCK_24;
    sInitTime.u8IsEnableWakeUp = 0;
    
    /* Initialization the RTC timer */
    RTC_Open(&sInitTime);
    RTC->TTR |= BIT3;// PowerDown wakeup on
    // Enable RTC Tick Interrupt 
    RTC_DisableInt(RTC_RIER_AIER);		
}

static void RtcStartWakeUpAlarm( uint32_t timeoutValue )
{
    time_t timestamp;
    struct tm unix_tm, *p_unix;
    UNLOCKREG();
    timeoutValue = timeoutValue;
    
//    TIMER_DisableWakeUp(TIMER1);
//    TIMER_DisableWakeUp(TIMER2);
//    TIMER_DeInit(TIMER1);
//    TIMER_DeInit(TIMER2);
//    TIMER_DisableWakeUp(TIMER1);
//    TIMER_DisableWakeUp(TIMER2);
    
    
    
    S_DRVRTC_TIME_DATA_T sCurTime;
    
    /* Get the currnet time */

    RTC_Read(&sCurTime);
    
    
    memset(&unix_tm, 0, sizeof(unix_tm));

    
    unix_tm.tm_sec = sCurTime.u32cSecond;
    unix_tm.tm_min = sCurTime.u32cMinute;
    unix_tm.tm_hour = sCurTime.u32cHour;
    unix_tm.tm_mday = sCurTime.u32cDay;
    unix_tm.tm_mon = sCurTime.u32cMonth;
    unix_tm.tm_year = sCurTime.u32Year - 1900;
    unix_tm.tm_wday = sCurTime.u32cDayOfWeek;
    timestamp = mktime(&unix_tm);
    
    
    timestamp += timeoutValue;
    
    p_unix = gmtime(&timestamp);
   
    sCurTime.u32cSecond = p_unix->tm_sec;
    sCurTime.u32cMinute = p_unix->tm_min;
    sCurTime.u32cHour = p_unix->tm_hour;
    sCurTime.u32cDay = p_unix->tm_mday;
    sCurTime.u32cMonth = p_unix->tm_mon;
    sCurTime.u32Year  = p_unix->tm_year + 1900;
    sCurTime.u32cDayOfWeek = p_unix->tm_wday;    
    RTC_WriteAlarm(&sCurTime);
    LOCKREG(); 
}

int main(void)
{
    UNLOCKREG();
    /* Enable HXT and LXT */
    SYS_SetChipClockSrc(CLK_PWRCTL_LIRC_EN, 1);
     //Wait HXT and LXT stable
    while(SYS_CheckChipClockSrc(CLK_CLKSTATUS_LIRC_STB) != 0) ;
    LOCKREG();
    //GPIO_Open(GPIOB, GPIO_PMD_PMD0_OUTPUT, GPIO_PMD_PMD0_MASK);
    //GPIO_ClrBit(GPIOB, 0);
    //for(int i = 0; i < 2000000; i++);
    //RTC_Init();	
    //RtcSetConfig();
    
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
    GCR->PF_L_MFP = 0;

    /* Enable GPIO pull up */
    GPIOA->PUEN = 0xFFFF;
    GPIOB->PUEN = 0xFFFF;
    GPIOC->PUEN = 0xFFFF;
    GPIOD->PUEN = 0xFFFF;
    GPIOE->PUEN = 0xFFFF;
    GPIOF->PUEN = 0x00FF;	
    for(int i = 0; i < 50000; i++);
    
    TimerHwInit_();
    
    
    while(1) {
        for(int i = 0; i < 1000000; i++);

        //RtcStartWakeUpAlarm(10);
        //for(int i = 0; i < 2000000; i++);
//        //GPIO_SetBit(GPIOB, 0);
//        //GPIO_ClrBit(LED1_PORT, LED1_PIN);
//        //TIMER_Stop(TIMER0);
//        //TIMER_Stop(TIMER1);
//        //TIMER_Stop(TIMER2);
        UNLOCKREG();
        //WDT_STOP;
        CLK->PWRCTL |= (CLK_PWRCTL_PWRDOWN_EN | CLK_PWRCTL_DELY_EN);   /* Set power down bit */
        SCB->SCR |= 0x04;						   /* Sleep Deep */
        __WFI();
        //WDT_START;
        LOCKREG();  
//        TIMER_Start(TIMER0);
//        TIMER_Start(TIMER1);
//        TIMER_Start(TIMER2);
//        GPIO_ClrBit(GPIOB, 0);

    }




}