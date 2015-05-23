#include "adc.h"
#include "../include/sysctrl.h"

/******************************************************************************
 * Initializes ADC to use Sample Sequencer #2, triggered by software, no IRQs
 *****************************************************************************/
void configure_adc0(void)
{
  ADC0_Type  *myADC;
  uint32_t rcgc_adc_mask;
  uint32_t pr_mask;
  
  rcgc_adc_mask=SYSCTL_RCGCADC_R0;
      // Set pr_mask
	pr_mask = SYSCTL_PRADC_R0;

  
  // Turn on the ADC Clock
  SYSCTL->RCGCADC |= rcgc_adc_mask;
  
  // Wait for ADCx to become ready
  while( (pr_mask & SYSCTL->PRADC) != pr_mask){}
    
  // Type Cast adc_base and set it to myADC
  myADC = (ADC0_Type *)ADC0_BASE;
  
  // ADD CODE
  // disable sample sequencer #2 by writing a 0 to the 
  // corresponding ASENn bit in the ADCACTSS register 
	myADC->ACTSS &= ~ADC_ACTSS_ASEN2; //need to set sth else?

  // ADD CODE
  // Set the event multiplexer to trigger conversion on a software trigger
  // for sample sequencer #2.
	myADC->EMUX &= ~ADC_EMUX_EM2_ALWAYS;

  // ADD CODE
  // Set IE0 and END0 in SSCTL2
	myADC->SSCTL2 = ADC_SSCTL2_IE2 | ADC_SSCTL2_END2 ; // may be wrong
  //revise
	 myADC->SSMUX2 = 0x201;          // Set the Channel, channel 2 + channel 0 + channel 1; p868
}

/******************************************************************************
 * Reads SSMUX2 for the given ADC.  Busy waits until completion
 *****************************************************************************/
/*bool get_adc_values( uint32_t adc_base, uint16_t *ps2_x, uint16_t *ps2_y)
{
  ADC0_Type  *myADC;
  //uint32_t result;
  
  if( adc_base == 0)
  {
    return false;
  }
  
  myADC = (ADC0_Type *)adc_base;
  
  myADC->SSMUX2 = 0x201;          // Set the Channel, channel 2 + channel 0 + channel 1; p868
  
  myADC->ACTSS |= ADC_ACTSS_ASEN2;  // Enable SS2
  
  myADC->PSSI =   ADC_PSSI_SS2;     // Start SS2
  
  while( (myADC->RIS & ADC_RIS_INR2)  == 0)
  {
    // wait
  }
  
  *ps2_x = myADC->SSFIFO2 & 0xFFF;  
   // Read 12-bit data
	*ps2_y = myADC->SSFIFO2 & 0xFFF;  

	myADC->SSFIFO2;  

	myADC->SSFIFO2;
	
  
  myADC->ISC  = ADC_ISC_IN2;          // Ack the conversion
  
  return true;
}*/
void trigger_adc()
{
	ADC0_Type  *myADC;
  // Type Cast adc_base and set it to myADC
  myADC = (ADC0_Type *)ADC0_BASE;
	
	myADC->ACTSS |= ADC_ACTSS_ASEN2;  // Enable SS2
  
  myADC->PSSI |=   ADC_PSSI_SS2;     // Start SS2
	
	myADC->IM |= ADC_IM_MASK2;  
	
	    // Set the priority to 1
  NVIC_SetPriority(ADC0SS2_IRQn, 1);
  
    // Enable the NVIC for the watchdog timer
  NVIC_EnableIRQ(ADC0SS2_IRQn);
}

