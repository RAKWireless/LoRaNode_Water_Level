/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#include <stdint.h>
#include "nano1xx.h"

//extern void RTC_TickHandle(void);
//extern void RTC_AlarmHandle(void);
//extern void RTC_SnooperHandle(void);

void TMR0_IRQHandler(void)
{
	TIMER0->ISR = 3;
  
}


void RTC_IRQHandler(void)
{ 
   
	if ( (RTC->RIER & RTC_RIER_TIER) && (RTC->RIIR & RTC_RIIR_TIS) )		/* tick interrupt occurred */
	{
 		  RTC->RIIR = 0x2;
       
         // RTC_TickHandle();
    }

 	if ( (RTC->RIER & RTC_RIER_AIER) && (RTC->RIIR & RTC_RIIR_AIS) )		/* alarm interrupt occurred */
    {          
		  RTC->RIIR = 0x1;
		  
		  //RTC_AlarmHandle();
    }

	if ( (RTC->RIER & RTC_RIER_SNOOPIER) && (RTC->RIIR & RTC_RIIR_SNOOPIS) )	/* snooper interrupt occurred */
    {          
		  RTC->RIIR = 0x4;
		  
		  //RTC_SnooperHandle();
    }
}


void GPABC_IRQHandler(void)
{
	GPIOA->ISR= GPIOA->ISR; /* clear interrupt */
}


void PDWU_IRQHandler(void)
{
 //// printf("     PDWU_IRQHandler running...\n");

 //// u32PWDU_WakeFlag = 1;
  CLK->WK_INTSTS = CLK_WK_INTSTS_IS; /* clear interrupt */
  
}


void WDT_IRQHandler(void)
{
	__IO uint32_t u32IntSts;

	u32IntSts = WDT->ISR;

	/*if (u32IntSts & WDT_ISR_WDTWAKEIS)
		printf("     WDT Wake Up Interrupt Occurs.\n");
	else
		printf("     WDT Interrupt Occurs <%d>.\n", u32WDT_Ticks);

	if (u32IntSts & WDT_ISR_WDTIS)
		u32WDT_Ticks++;*/
  WDT->CTL |=WDT_CTL_WTR;
	WDT->ISR = u32IntSts; /* clear interrupts */
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

