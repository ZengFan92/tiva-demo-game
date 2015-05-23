#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "TM4C123.h"
#include "boardUtil.h"
#include "pc_buffer.h"
#include "eeprom.h"
#include "wireless.h"
#include "marios.h"
#include "game.h"
#include "fonts.h"
#include "lcd.h"
#include "watchdog0.h"



#define   DAC_BASE    GPIOB_BASE
#define 	PS2_Rotation GPIOE_BASE
#define   Push_Buttons  GPIOF_BASE
#define   PS2_Buttons  GPIOE_BASE
#define   joymeter (1 << 1)
#define   Xdir     (1 << 2)
#define   Ydir     (1 << 3)
#define   PS2_X_ADC_CHANNEL  1
#define   PS2_Y_ADC_CHANNEL  0
#define   clockfreq  50000  // 50MHz = 50,000 kHz
#define   maxMeter 0xFFF
#define   DAC0   (1 << 0)
#define   DAC1   (1 << 1)
#define   DAC2   (1 << 2)
#define   DAC3   (1 << 3)
#define   DAC4   (1 << 4)
#define   DAC5   (1 << 5)
#define DAC_PIN DAC0 | DAC1 | DAC2 | DAC3 | DAC4 | DAC5

#define   UP 		 (1 << 1)
#define   RIGHT 		 (1 << 2)
#define   LEFT 		 (1 << 3)
#define   DOWN 		 (1 << 4)

#define   JOYPUSH 		 (1 << 0)




/******************************************************************************
 * Global Variables
 *****************************************************************************/
	char name1[80];
	char name2[80];
	char teamNumber[2];
	char input[80];
	static uint8_t read_data[80];
	
	volatile bool Alert_1ms = false;// Alert the systick interrupt
	volatile int Alert_100ms = 0;// Alert the systick interrupt(100ms)
	volatile bool Alert_Timer0A = false;// Alert the timerA interrupt 
	volatile bool Alert_Timer0A5s = false;// Alert the timerA interrupt in 5 sec
	volatile int Alert_count = 0;// Count the cycles of the timerA
	volatile bool Alert_Timer1B;
	volatile bool Alert_Timer1A = false;
	static int playSound = 0;
 volatile bool ps2IsPress = false;
 volatile bool upIsPress = false;
 volatile bool downIsPress = false;
volatile bool rightIsPress = false;
	volatile bool leftIsPress = false;
	volatile bool Alert_100ms2 = false;
	volatile	int package = 0; //  the amount of package
	volatile int loss =0;//  the amount of lossed package
	//volatile uint32_t data =0;
	static uint32_t data_r = 0;// recieved
	volatile uint32_t data_rr = 0;//to be sent
	volatile int spicount = 0;
volatile uint16_t y_data;
volatile uint16_t x_data;
	
	
		volatile int tracker = 4; // the current location of the tracker
		volatile int direction; // the direction of the joystick
		volatile int enemy =4; // the location of the enemy

		volatile int score;
		volatile int endgame;
		volatile bool tx_mode = false;
		
//	uint16_t x_data, y_data;
	uint8_t myID[]      = { '2', '5', '7', '8', '6'};
uint8_t remoteID[]  = { '2', '4', '7', '6', '4'};
char titleP1[] = {'P','1','-','U','P',' ',' '};
char titleP2[] = {'P','2','-','D','O','W','N'};
char winMessage[] = {'Y','o','u',' ','w','i','n'};
char loseMessage[] = {'Y','o','u',' ','l','o','s','e'};
volatile int scene = 0;// 0 for title 1,for 1p, 2 for 2p

//*****************************************************************************
//*****************************************************************************
void initializeBoard(void)
{
  int i;
	char input2[80] ={0};
	uint16_t x_data, y_data;
  DisableInterrupts();
  serialDebugInit();
	i2cInit();
	rfInit();
  EnableInterrupts();
	//************************
	//Setting up buttons
		// enable push button as digital input
	gpio_enable_port(Push_Buttons);
	gpio_config_enable_input(Push_Buttons, UP|DOWN|RIGHT|LEFT);
	gpio_config_digital_enable(Push_Buttons, UP|DOWN|RIGHT|LEFT);
	gpio_config_enable_pullup(Push_Buttons, UP|DOWN|RIGHT|LEFT);
	
			// enable PS2 button as digital input
	gpio_enable_port(PS2_Buttons);
	gpio_config_enable_input(PS2_Buttons, JOYPUSH);
	gpio_config_digital_enable(PS2_Buttons, JOYPUSH);
	gpio_config_enable_pullup(PS2_Buttons, JOYPUSH);

	// enable Joystick and potentialmeter as analog input
	gpio_enable_port(PS2_Rotation);
	gpio_config_enable_input(PS2_Rotation, joymeter  | Xdir | Ydir);
	gpio_config_analog_enable(PS2_Rotation, joymeter  | Xdir | Ydir);
	gpio_config_alternate_function(PS2_Rotation, joymeter  | Xdir | Ydir);
	// ADC0
	configure_adc0();
	// Timer
	configure_timer0();
	configure_timer1();

		// enable DAC as digital output
	gpio_enable_port(DAC_BASE);
	gpio_config_enable_output(DAC_BASE, DAC_PIN);
	gpio_config_digital_enable(DAC_BASE, DAC_PIN);
	// lcd config
	lcd_init();
	// config watchdog timer
	configure_WDT0();
	
	
	
	//*****************
	//The following is to intialize the fields in eeprom
	// Comment after initialization

//		input2[0] = 'Z';
//	input2[1] = 'e';
//	input2[2] = 'n';
//	input2[3] = 'g';
//	input2[4] = 0;
//		  for(i = 0; i < 80; i++)
//  {
//    
//    eeprom_byte_write(EEPROM_I2C_BASE,i,input2[i]);
//  }
//	input2[0] = 'K';
//	input2[1] = 'e';
//	input2[2] = 'r';
//	input2[3] = 'e';
//	input2[4] = 'n';
//		  for(i = 0; i < 	80; i++)
//  {
//    
//    eeprom_byte_write(EEPROM_I2C_BASE,i+80,input2[i]);
//  }
//	input2[0] = '0';
//	input2[1] = '7';
//	input2[2] = '0';
//	input2[3] = 0;
//	input2[4] = 0;
//	 
//			  for(i = 0; i < 2; i++)
//  {
//    
//    eeprom_byte_write(EEPROM_I2C_BASE,i+160,input2[i]);
//  }
//	eeprom_byte_write(EEPROM_I2C_BASE,163,0);
   
}
//*****************************************************************************
//*****************************************************************************
// width 37, 6pages
void printXueyou(uint16_t offset)
{
	  int i,j;
  
  dogs102_init();

  dogs102_clear();
  
  for(i = 0; i < 6; i++)
  {
    
     dogs102_set_page(i+1);
    for(j=0; j<37; j++)
    {
      dogs102_set_column(j+offset);
      dogs102_write_data(xueyou[i*37 + j]);
    }
  }
}
//*****************************************************************************
//*****************************************************************************
// //mario3WidthPixels = 48;
//mario3HeightPages = 8;
void printMario(uint16_t offset)
{
	int i,j;
  
  dogs102_init();

  dogs102_clear();
  
  for(i = 0; i < 8; i++)
  {
    
     dogs102_set_page(i);
    for(j=0; j<48; j++)
    {
      dogs102_set_column(j+offset);
      dogs102_write_data(mario8Bit[i*48 + j]);
    }
  }
}
//*****************************************************************************
//*****************************************************************************
void printTodd(uint16_t offset)
{
	  int i,j;
  
  dogs102_init();

  dogs102_clear();
  
  for(i = 0; i < 7; i++)
  {
    
     dogs102_set_page(i+1);
    for(j=0; j<56; j++)
    {
      dogs102_set_column(j+offset);
      dogs102_write_data(todd[i*56 + j]);
    }
  }
}



//*****************************************************************************
//*****************************************************************************
void readOldFields()
{
	int i;
	
	  for(i = 0; i < 80; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i,&(read_data[i]));
  }
	printf("\n\r Current: your name: %s ", read_data);
		  for(i = 0; i < 80; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i+80,&(read_data[i]));
  }
	printf("\n\r Current: your partener's name: %s ", read_data);
		  for(i = 0; i < 2; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i+160,&(read_data[i]));
  }
	printf("\n\r Current: your team number: %c%c ", read_data[0], read_data[1]);
	for(i = 0; i < 1; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i+163,&(read_data[i]));
  }
	printf("\n\r Current: your wins: %i ", read_data[0]);

}
//*****************************************************************************
// Enter new information to the Eeprom
//*****************************************************************************
// Ask the user through Uart about their new information
//  And then save to Eeprom
void enterNewFields()
{
	int i;
	int wins;
	  memset(input,0,80);
  printf("\n\rEnter your name: ");
  scanf("%79[^\n]", input);
	memcpy(name1, input, 80);
 // printf("You entered: %s\n\r",name1);
	printf("\n\rEnter your partener's name: ");
  scanf("%79[^\n]", input);
	memcpy(name2, input, 80);
  //printf("You entered: %s\n\r",name2);
	printf("\n\rEnter your team number: ");
	
  scanf("%79[^\n]", input);
	memcpy(teamNumber, input, 2);
	
	  for(i = 0; i < 80; i++)
  {
    
    eeprom_byte_write(EEPROM_I2C_BASE,i,name1[i]);
  }
		  for(i = 0; i < 80; i++)
  {
    
    eeprom_byte_write(EEPROM_I2C_BASE,i+80,name2[i]);
  }
			  for(i = 0; i < 2; i++)
  {
    
    eeprom_byte_write(EEPROM_I2C_BASE,i+160,teamNumber[i]);
  }
	
	}


//*****************************************************************************
	//joystick usage
//*****************************************************************************
	//Return: 0 - Up
	//        1 - Right
	//        2 - Left
	//        3 - Down
	//        4 - null
	int joystick()
	{
		//get_adc_values(ADC0_BASE,&x_data,&y_data);
		if(y_data < 0.1 * maxMeter)
		{
			return 3;
		}
		else if(x_data < 0.1 * maxMeter)
		{
			return 1;
		}
		else if(y_data>0.9* maxMeter)
		{
			return 0;
		}
		else if(x_data >0.8*maxMeter)
		{
			return 2;
		}
		else
		{
			return 4;
		}
	}
	
	
	//*********************************************************************
	//Sound effect configuration
	//*********************************************************************
	void configureSound()
	{
		
		start_timer1B(11337);
		
	}
	//*********************************************************************
	//Use timer1 to show the title and animation
	//*********************************************************************
void gotoTitle()
{
int i,j;
	printTodd(0);
	start_timer1A(32767);
	
	for(i = 0; i<1000;i++)
	{
	
	while(!Alert_Timer1A)
	{	
	}

			
	Alert_Timer1A=false;
	
	
}
	dogs102_clear();	
	printTodd(20);
	for(i = 0; i<1000;i++)
	{
	
	while(!Alert_Timer1A)
	{	
	}

			
	Alert_Timer1A=false;
	
	
	}
	dogs102_clear();
	Alert_Timer1A=false;
	printTodd(40);
	for(i = 0; i<1000;i++)
	{
	
	while(!Alert_Timer1A)
	{	
	}

			
	Alert_Timer1A=false;
	
	
	}
	Alert_Timer1A=false;
	dogs102_clear();
	stop_timer1A();
	
	for(i=0;i<7; i++)
	{
			dogs102_write_char_10pts(1,titleP1[i],i);
			dogs102_write_char_10pts(2,titleP2[i],i);
	}
}
//*********************************************************************
//Use timer1 to show the winner ending
//*********************************************************************
void gotoWinEnding()
{
	int i,j;
	printMario(30);
	stop_timer1A();
	start_timer1A(32767);
	for(i = 0; i<1000;i++)
	{
	
		while(!Alert_Timer1A)
		{	
		}

			
		Alert_Timer1A=false;
	
	
	}
	Alert_Timer1A=false;
	dogs102_clear();
	stop_timer1A();
	for(i=0;i<7; i++)
	{
			dogs102_write_char_10pts(1,winMessage[i],i);
	}
}
//*********************************************************************
//Use timer1 to show the loser ending
//*********************************************************************
void gotoLoseEnding()
{
	int i,j;
	printXueyou(30);
	stop_timer1A();
	start_timer1A(32767);
	for(i = 0; i<1000;i++)
	{
	
	while(!Alert_Timer1A)
	{	
	}

			
	Alert_Timer1A=false;
	
	
	}
	Alert_Timer1A=false;
	dogs102_clear();
	stop_timer1A();

	
	for(i=0;i<8; i++)
	{
			dogs102_write_char_10pts(1,loseMessage[i],i);
	}
}
//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	//char msg[80];
  wireless_com_status_t status;
  bool   watchdogBegin = false; // enable watchdog while seeking enemy
  uint32_t data;
  int i = 0;
	int k =0;
	uint32_t loc =4;
	bool   tx_mode;


	float lossRate=0;
	int soundcount = 0;
	GPIOA_Type *dacPort = (GPIOA_Type *) DAC_BASE;	
  initializeBoard();
  scene = -1;
	endgame = 0;
	
	tx_mode = false;

	//dogs102_write_char_10pts(2,'c',5);  //print on LCD
	readOldFields();
  gotoTitle();
	scene =0;
	score = 0;
	
	
		// Systick timer 1 ms
	SysTick_Config(50000);
	//Start timerA 0.5ms
	start_timer0A(25000);
  // Infinite Loop
	start_timer1A(32000);
	//enable_watchdog();

  while(scene != 20)
		{

			

				
			
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			//THE game code in the MAIN
				if(Alert_100ms2 && ((scene == 1) ||(scene == 2)))
					//ADD CODE DISABLE WATCHDOG
				{


			
					Alert_100ms2 = false;
					loc = data_r&(0x0F);

					updateEnemy(loc);
					//printf("\n\r enemy at %i",loc);
					
					updateGame(direction, rightIsPress);
					clearCursor();
					printToad();
					printBlack();
					//printf("\n\r enemy at %i",data_r&0x0F);
					if(endgame != 0)
					{
						scene = 9;
						//disable watchdog
						//SYSCTL->SRWD = 0x01;
						renew_watchdog();
					}
					if(((data_r ==0x0F1) || (data_r ==0x0F2))&& (scene !=9))
					{
						scene = 10;
						//disable watchdog
						//SYSCTL->SRWD = 0x01;
						renew_watchdog();
						printf("\n \r scene 10");
						if(data_r == 0x0F1)
						{
						endgame  = 2;
						}
						else
						{
							endgame  = 1;
						}
						data_rr = 0x0F3;
					}

				}

				
				
				
				//******************************************
				//When timerA interrupt
				//******************************************
				//print wireless packet statisitics every 5s
				//start ADC
				if(Alert_Timer0A)// When timer A terminate
				{

					Alert_Timer0A = false;
					direction = joystick();
					//  Figure out the where is the joystick
					//  into 4 directions

					///*********************************
					//Print the package statistics
					
					if(Alert_Timer0A5s)
					{
						
						Alert_Timer0A5s = false;
						//TODO printing the packet statistics
						
						if(package == 0)
						{
							printf("\n\r No package tranmitted last 5 seconds");
						}
						else
						{
							if(loss!=package)  //if received any data, renew watchdog
							{
								renew_watchdog();
							}
							lossRate = loss/package;
							printf("\n\r Package loss: %f",lossRate);
						
						}
						package=0;
						loss=0;
						if(scene == 10)  //p2,Alert_100ms = 60ms
						{
							k++;
							if(k >0)
							{
								scene = 20;
								k=0;
							}
						}
					}
					
					//******************************

				}
				if(upIsPress && (scene == 0))
				{
					wireless_configure_device(myID, remoteID ) ;
					scene = 3;
					
				}
				else if(downIsPress && (scene == 0))
				{
					wireless_configure_device( remoteID,myID ) ;
					scene = 4;
				}

		//*************************************************************************
		//  Recieve msg
		//ADD CODE (CLR)
				if(Alert_Timer1A && tx_mode)
			{
				Alert_Timer1A= false;
				status =  wireless_get_32(false, &data);
									if(status == NRF24L01_RX_SUCCESS)
							{
								data_r =  data;
									//printf("1shake Received: %d\n\r", data);
							}
					
					
				}
			//************************************************************************
				//  send msg
				//ADD CODE (CLR)
			if((spicount == 0)&& tx_mode)
			{
				if((scene == 2)||(scene == 1) ||(scene == 10))
				{
				status = wireless_send_32(false, false, data_rr);// allow package loss in the game
				}
				else
				{
					status = wireless_send_32(false, true, data_rr);//do not allow package loss in shanking hands and "end game " communication
				}
				package++;
				if (status != NRF24L01_TX_SUCCESS)
				{
					loss++;
					//printf("sending: fail \n\r" );
				}
			}

				
			//Alert_Timer1A= false;
			if(scene == 3)
			{
				if(!watchdogBegin)  //if watchdog is not enable, enable watchdog
				{
					enable_watchdog();
					watchdogBegin = false;
				}
				printf(" \r Connecting to 2P...");
				tx_mode = true;
				//printf("\n \r scene 3");
				data_rr = 0x00;
				if(data_r == 0x01)
				{
					scene = 5;
					renew_watchdog();
				}
			
			}
			if(scene == 4)
			{
				tx_mode = true;
				if(!watchdogBegin)  //if watchdog is not enable, enable watchdog
				{
					enable_watchdog();
					watchdogBegin = false;
				}
				printf(" \r Connecting to 1P...");
				if(data_r == 0x00)
				{
					scene = 6;
					renew_watchdog();
				}
	
			}
			if(scene == 5)
			{
				data_rr = 0xA0;
				if(data_r == 0x01)
				{
					scene = 1;
					printf("\r Connection established");		
					dogs102_clear();  // clear LCD
					printBoard();  // col +20
					renew_watchdog();
				}
						
			}
			if(scene == 6)
			{
				data_rr = 0x01;
				if((data_r & 0X0F0) == 0xA0)
				{
					printf("\n \r Connection established");
					scene = 2;
					dogs102_clear();  // clear LCD
					printBoard();  // col +20
					renew_watchdog();
				}
			}
					
					
					//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
					//end game
			if(scene == 9)  //p1
			{
				if(endgame == 1)
				{
					data_rr = 0x0F1;
				}
				else
				{
					data_rr = 0x0F2;
				}
				if(data_r == 0x0F3)
				{
					scene = 20;
				//dogs102_clear();  // clear LCD
				}
						
			}
								
				/*if(Alert_100ms2 && scene == 10)  //p2,Alert_100ms = 60ms
				{
						k++;
						if(k >400)
						{
							scene = 20;
							k=0;
						}
				}*/
		}
		//disable watchdog
		SYSCTL->SRWD = 0x01;
		dogs102_clear();
		//eeprom_byte_read(EEPROM_I2C_BASE,163,&(read_data[0]));
		
		stop_timer0A();
		stop_timer1B();
		stop_timer1A();
		playSound = 1;
		if(endgame==1) // you win
		{
			gotoWinEnding();
			read_data[0]++;
			eeprom_byte_write(EEPROM_I2C_BASE,163,read_data[0]);
		}
		if(endgame==2) //
		{
			gotoLoseEnding();
		}
		// eeprom_byte_write(EEPROM_I2C_BASE,163,read_data[0]);
		configureSound();
		while(1)
		{

				if(Alert_Timer1B)
				{
					Alert_Timer1B = false;
					if(playSound == 1)
					{
					dacPort->DATA = mario[soundcount]*2;
					soundcount++;
					//printf("dac");
					if(soundcount > 8640)
					{
						soundcount=0;
						stop_timer1B();
						NVIC_SystemReset();
					}
				}
				
			}
		}
}

