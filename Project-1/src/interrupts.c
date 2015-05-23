#include "interrupts.h"
#include "timer0.h"
#include "main.h"
#include "boardUtil.h"
#include "lcd.h"
#include "game.h"
#include "wireless.h"
#include "spi.h"

extern PC_Buffer UART0_Rx_Buffer;
extern PC_Buffer UART0_Tx_Buffer;

//*****************
extern volatile bool Alert_1ms;
extern volatile bool Alert_Timer0A;
extern volatile int Alert_count;
extern volatile bool Alert_Timer0A5s;
extern volatile int Alert_100ms;
extern volatile bool Alert_100ms2;

extern volatile bool Alert_Timer1B;
extern volatile bool Alert_Timer1A;
extern volatile bool ps2IsPress ;
extern volatile bool upIsPress ;
extern volatile bool downIsPress ;
extern volatile bool rightIsPress ;
extern	volatile bool leftIsPress;
extern volatile int scene;
extern volatile uint16_t y_data;
extern volatile uint16_t x_data;
static int Alert_600msCount = 0;
	static int ps2count = 0;
	static int upcount = 0;
	static int downcount = 0;
	static int leftcount = 0;
	static int rightcount = 0;

	extern volatile uint32_t data_r;
extern volatile bool tx_mode;
extern volatile int spicount;
#define   UP 		 (1 << 1)
#define   RIGHT 		 (1 << 2)
#define   LEFT 		 (1 << 3)
#define   DOWN 		 (1 << 4)

#define   JOYPUSH 		 (1 << 0)


//*****************************************************************************
// Used to debounce the pushbutton and check the potentiometer.
//*****************************************************************************
void SysTick_Handler(void)
{
  uint32_t val;
  
  Alert_1ms = true;
	
						if((GPIOE->DATA & JOYPUSH) == 0)
					{
						ps2count++;
					}
					else
					{
						ps2count = 0;
						ps2IsPress = false;
					}
					if(ps2count >= 16 && !ps2IsPress)
					{
						ps2IsPress = true;
					}

						//End of checking PS2 button

					if((GPIOF->DATA & UP) == 0)
					{
						upcount++;
					}
					else
					{
						upcount = 0;
						upIsPress = false;
					}
					if(upcount >= 16 && !upIsPress)
					{
						upIsPress = true;
					}
					if((GPIOF->DATA & DOWN) == 0)
					{
						downcount++;
					}
					else
					{
						downcount = 0;
						downIsPress = false;
					}
					if(downcount >= 16 && !downIsPress)
					{
						downIsPress = true;
					}
					
					if((GPIOF->DATA & LEFT) == 0)
					{
						leftcount++;
					}
					else
					{
						leftcount = 0;
						leftIsPress = false;
					}
					if(leftcount >= 16 && !leftIsPress)
					{
						leftIsPress = true;


					}
					
					if((GPIOF->DATA & RIGHT) == 0)
					{
						rightcount++;
					}
					else
					{
						rightcount = 0;
						rightIsPress = false;
					}
					if(rightcount >= 16 && !rightIsPress)
					{
						rightIsPress = true;
					}

				//***************
			//Checking the PS2 button and push buttons  //debounce PS2button and reset board
			
						if(ps2IsPress)
					{
							DisableInterrupts();
							//disable watchdog
							SYSCTL->SRWD = 0x01;
							readOldFields();
							enterNewFields();
							//reboot the board
							NVIC_SystemReset();
							//ps2IsPress = false;
							//EnableInterrupts();
					}
			//*************************************************************
  // Clear the interrupt
  val = SysTick->VAL;
}



//*****************************************************************************
// Rx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Rx_Flow(PC_Buffer *rx_buffer)
{
  // Remove entries from the RX FIFO until the HW FIFO is empty.
  // Data should be placed in the rx_buffer.  

  // Clear the RX interrupts so it can trigger again when the hardware 
  // FIFO becomes full
		char data;
	while(!(UART0->FR &UART_FR_RXFE))
	{
		
		data = UART0->DR;

		pc_buffer_add(rx_buffer, data);
		
	}
	UART0->ICR = UART_ICR_RXIC;

}

//*****************************************************************************
// Tx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Tx_Flow(PC_Buffer *tx_buffer)
{
      char c;
  
        // Check to see if we have any data in the circular queue
  
            // Move data from the circular queue to the hardware FIFO
            // until the hardware FIFO is full or the circular buffer
            // is empty.

            // Any data in the hardware FIFO will continue to be transmitted
		while((!(UART0->FR &UART_FR_TXFF)) && !(pc_buffer_empty(tx_buffer)))
	{
		pc_buffer_remove(tx_buffer, &c);
		UART0->DR = c;

		
		
	}
 
        
        // Clear the TX interrupt so it can trigger again when the hardware
        // FIFO is empty
	UART0->ICR = UART_ICR_TXIC;

}

//*****************************************************************************
// UART0 Interrupt Service handler
//*****************************************************************************
void UART0_Handler(void)
{
    uint32_t  status;

    // Check to see if RXMIS or RTMIS is active
    status = UART0->MIS;

    if ( status & (UART_MIS_RXMIS | UART_MIS_RTMIS ) )
    {
       UART0_Rx_Flow(&UART0_Rx_Buffer);
    }

    // Check the TX interrupts
    if ( status & UART_MIS_TXMIS )
    {
      UART0_Tx_Flow(&UART0_Tx_Buffer);
    }
    return;
}



//*********************************************************
//TimerA Interupt Service handler
void TIMER0A_Handler(void)
{
  Alert_Timer0A = true;
	  Alert_count++;
	Alert_100ms++;
	//CommentsADC. trigger ADC conversions
	trigger_adc();
	
		if(Alert_100ms >120)
	{
		// zeng print board

		Alert_100ms =0;
		Alert_100ms2= true;

	}
			if(Alert_count >10000)
	{
		// zeng print board

		Alert_count =0;
		Alert_Timer0A5s= true;

	}


  TIMER0->ICR |= TIMER_ICR_TATOCINT;

}


//*****************************************************************************
// Timer0B Interrupt Handler
// Used to time the "OFF" portion of the waveform
//*****************************************************************************
void TIMER1A_Handler(void)
{
  Alert_Timer1A = true;
  
	spicount++;
	if(spicount >200)
	{
		spicount = 0;
		

	}
		


	TIMER1->ICR |= TIMER_ICR_TATOCINT;
}
void TIMER1B_Handler(void)
{
  Alert_Timer1B = true;
  TIMER1->ICR |= TIMER_ICR_TBTOCINT;
}
//****************************************
//Watchdog Interupt Service handler
void WDT0_Handler(void)
{
	printf("\nboard is halted");
}
//*****************************************************************************
// ADCSS2 Interrupt Handler
// Used to get ADC value
//*****************************************************************************
void ADC0SS2_Handler(void)
{
	 ADC0_Type  *myADC;
   // Type Cast adc_base and set it to myADC
   myADC = (ADC0_Type *)ADC0_BASE;
	 x_data = myADC->SSFIFO2 & 0xFFF;  
   // Read 12-bit data
	 y_data = myADC->SSFIFO2 & 0xFFF;  
	 myADC->ISC  = ADC_ISC_IN2;          // Ack the conversion
}