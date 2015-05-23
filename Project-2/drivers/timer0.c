#include "timer0.h"

//*****************************************************************************
// Configure a 16/32 bit general purpose timer to wait a specified number
// of clock cycles
//*****************************************************************************
void configure_timer1()
{
  TIMER0_Type *gp_timer = ( TIMER0_Type *) TIMER1_BASE;
  uint32_t timer_rcgc_mask;
  uint32_t timer_pr_mask;
  
  // Set the timer_rcgc_mask and timer_pr_mask using the appropriate
  // #defines in ../include/sysctrl.h

  timer_rcgc_mask = SYSCTL_RCGCTIMER_R1;
  timer_pr_mask = SYSCTL_PRTIMER_R1;

  
  // Turn on the clock for the timer
  SYSCTL->RCGCTIMER |= timer_rcgc_mask;

  // Wait for the timer to turn on
  while( (SYSCTL->PRTIMER & timer_pr_mask) == 0) {};
	// Timer0A:Set the priority to 1
	// Enable the NVIC for the watchdog timer

	// TimerA
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	//gp_timer->CFG = TIMER_CFG_32_BIT_TIMER;;
		gp_timer->CFG = TIMER_CFG_16_BIT;
	gp_timer->TAMR = TIMER_TAMR_TAMR_PERIOD;
	//gp_timer->CTL |= TIMER_CTL_TAEN;
	gp_timer->IMR = TIMER_IMR_TATOIM | TIMER_IMR_TBTOIM;
	//	gp_timer->ICR |= TIMER_ICR_TATOCINT|TIMER_ICR_TBTOCINT;
		  NVIC_SetPriority(TIMER1A_IRQn, 1);
  NVIC_EnableIRQ(TIMER1A_IRQn);
	// TimerB
  NVIC_SetPriority(TIMER1B_IRQn, 1);
  NVIC_EnableIRQ(TIMER1B_IRQn);	
	gp_timer->CTL &= ~TIMER_CTL_TBEN;

	gp_timer->TBMR = TIMER_TBMR_TBMR_PERIOD;
}
  
  
void start_timer1A(uint16_t load_value)
{
	
	TIMER0_Type *gp_timer = ( TIMER0_Type *) TIMER1_BASE;
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	gp_timer->TAILR = load_value;

	gp_timer->CTL |= TIMER_CTL_TAEN;

	
}
void start_timer1B(uint16_t load_value)
{
	
	TIMER0_Type *gp_timer = ( TIMER0_Type *) TIMER1_BASE;
	gp_timer->CTL &= ~TIMER_CTL_TBEN;
	gp_timer->TBILR = load_value;

	gp_timer->CTL |= TIMER_CTL_TBEN;

}
//*****************************************************************************
// Turns on Timer0B and Turns Off Timer0A.  The TBILR is set to load_value
//*****************************************************************************

void stop_timer1A(void)
{
	TIMER0_Type *gp_timer = ( TIMER0_Type *) TIMER1_BASE;

	gp_timer->CTL &= ~TIMER_CTL_TAEN;
}

void stop_timer1B(void)
{
	TIMER0_Type *gp_timer = ( TIMER0_Type *) TIMER1_BASE;

	gp_timer->CTL &= ~TIMER_CTL_TBEN;
}

//*****************************************************************************
// Configure a 32 bit general purpose timer to wait a specified number
// of clock cycles
//*****************************************************************************
//CommentsTimer
void configure_timer0()
{
  TIMER0_Type *gp_timer = ( TIMER0_Type *) TIMER0_BASE;
  uint32_t timer_rcgc_mask;
  uint32_t timer_pr_mask;
  
  // Set the timer_rcgc_mask and timer_pr_mask using the appropriate
  // #defines in ../include/sysctrl.h

  timer_rcgc_mask = SYSCTL_RCGCTIMER_R0;
  timer_pr_mask = SYSCTL_PRTIMER_R0;

  
  // Turn on the clock for the timer
  SYSCTL->RCGCTIMER |= timer_rcgc_mask;

  // Wait for the timer to turn on
  while( (SYSCTL->PRTIMER & timer_pr_mask) == 0) {};
	// Timer1A:Set the priority to 1
	// Enable the NVIC for the watchdog timer

	// TimerA
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	gp_timer->CFG = TIMER_CFG_32_BIT_TIMER;
	gp_timer->TAMR = TIMER_TAMR_TAMR_PERIOD;
	//gp_timer->CTL |= TIMER_CTL_TAEN;
	gp_timer->IMR = TIMER_IMR_TATOIM ;
	//	gp_timer->ICR |= TIMER_ICR_TATOCINT|TIMER_ICR_TBTOCINT;
		  NVIC_SetPriority(TIMER0A_IRQn, 1);
  NVIC_EnableIRQ(TIMER0A_IRQn);
}
  
  
void start_timer0A(uint32_t load_value)
{
	
	TIMER0_Type *gp_timer = ( TIMER0_Type *) TIMER0_BASE;
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	gp_timer->TAILR = load_value;

	gp_timer->CTL |= TIMER_CTL_TAEN;

	
}

//*****************************************************************************
// Turns on Timer1B and Turns Off Timer1A.  The TBILR is set to load_value
//*****************************************************************************

void stop_timer0A(void)
{
	TIMER0_Type *gp_timer = ( TIMER0_Type *) TIMER0_BASE;

	gp_timer->CTL &= ~TIMER_CTL_TAEN;
}


