#include "watchdog0.h"
// configure a watchdog timer
//CommentsWDT
void configure_WDT0()
{
	WATCHDOG0_Type *wdt = ( WATCHDOG0_Type *) WATCHDOG0;
	// set rcgc mask and pr mask
	uint32_t wdt_rcgc_mask = SYSCTL_RCGCWD_R0;
	uint32_t wdt_pr_mask = SYSCTL_PRWD_R0;
	
	// Turn on the clock for the watchdog timer
	SYSCTL->RCGCWD |= wdt_rcgc_mask;
	// Wait for the timer to turn on
  while( (SYSCTL->PRTIMER & wdt_pr_mask) == 0) {};
	// clear interrupt
	//wdt->ICR = 0x01;
	// follow p.777 12.3 Initialization and Configuration
		// set the load
	//	wdt->CTL = 0x 00 ;
	//wdt->LOAD = 500000000; //10s=500,000,000 ticks
//		wdt->CTL = 0x01 ;
//		NVIC_SetPriority(WATCHDOG0_IRQn,0);
//	NVIC_EnableIRQ(WATCHDOG0_IRQn);
//		wdt->LOCK = 0x01;
		
}	
//CommentsWDT: enable watchdog with load 10s
void enable_watchdog()
{
	WATCHDOG0_Type *wdt = ( WATCHDOG0_Type *) WATCHDOG0;
	// follow p.777 12.3 Initialization and Configuration
	//Set the INTEN bit in the WDTCTL register to enable the Watchdog, enable interrupts, 
	// and lock the control register.
	wdt->LOAD = 500000000;
	wdt->CTL |= 0x01 ; // Set the INTEN bit in the WDTCTL register
	// enbale interrupts
	NVIC_SetPriority(WATCHDOG0_IRQn,0);
	NVIC_EnableIRQ(WATCHDOG0_IRQn);
	// lock the control register
	//Writing 0x1ACC.E551 to the WDTLOCK register enables 
	//write access to all other registers.
	//Writing
	//any other value to the WDTLOCK register re-enables the locked state for register writes to all the
 //for register writes to all the other registers
	//wdt->LOCK = 0x1ACCE551;
}

void renew_watchdog()
{
	WATCHDOG0_Type *wdt = ( WATCHDOG0_Type *) WATCHDOG0;
	wdt->LOAD = 500000000;
}