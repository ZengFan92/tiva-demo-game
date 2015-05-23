#include <stdbool.h>
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcd.h"
#include "timer0.h"

extern volatile int tracker; // the current location of the tracker

extern	volatile int enemy; // the location of the enemy

extern	volatile int score;
extern volatile int scene;
extern volatile uint32_t data_rr;
extern volatile int endgame;
// This is the 3*3 matrix board
//*****************************************************************************
//*****************************************************************************
void clearCursor()
{
	  int i,j;
  
  
  for(i = 0; i < 6; i++)
  {
    
     dogs102_set_page(i+1);
    for(j=0; j<48; j++)
    {
      dogs102_set_column(j+25);
      dogs102_write_data(board[i*48 + j]);
    }
  }
	
}

//*****************************************************************************
//*****************************************************************************
void printBoard()
{
	  int i,j;
  
  dogs102_init();

  dogs102_clear();
  
  for(i = 0; i < 6; i++)
  {
    
     dogs102_set_page(i+1);
    for(j=0; j<48; j++)
    {
      dogs102_set_column(j+25);
      dogs102_write_data(board[i*48 + j]);
    }
  }
}

//*****************************************************************************
//*****************************************************************************
void printToad()
{
	  int row,col,i,j, temp;
  

	
		if(scene == 1)
	{
		temp = enemy;
	}
	if(scene == 2)
	{
		temp = tracker;
	}
  switch(temp)
	{
		case 0:
			row = 0;
			col = 0;
			break;
		case 1:
			row = 0;
			col = 1;
			break;
		case 2:
			row = 0;
			col = 2;
			break;
		case 3:
			row = 1;
			col = 0;
			break;
		case 4:
			row = 1;
			col = 1;
			break;
		case 5:
			row = 1;
			col = 2;
			break;
		case 6:
			row = 2;
			col = 0;
			break;
		case 7:
			row = 2;
			col = 1;
			break;
		case 8:
			row = 2;
			col = 2;
			break;
		default:
			break;
	}
			
  for(i = 0; i < 2; i++)
  {
    
     dogs102_set_page(i+2*row+1);
    for(j=0; j<16; j++)  // cursor width = 16
    {
      dogs102_set_column(j+25+col*16);
      dogs102_write_data(littleToad[i*16+j]);
			//dogs102_write_data(0xFF);
    }
  }
}

//*****************************************************************************
//*****************************************************************************
void printBlack()
{
	  int row,col,i,j,temp;
  
  //dogs102_init();

  //dogs102_clear();
	//clearCursor();
	if(scene == 1)
	{
		temp = tracker;
	}
	if(scene == 2)
	{
		temp = enemy;
	}
  switch(temp)
	{
		case 0:
			row = 0;
			col = 0;
			break;
		case 1:
			row = 0;
			col = 1;
			break;
		case 2:
			row = 0;
			col = 2;
			break;
		case 3:
			row = 1;
			col = 0;
			break;
		case 4:
			row = 1;
			col = 1;
			break;
		case 5:
			row = 1;
			col = 2;
			break;
		case 6:
			row = 2;
			col = 0;
			break;
		case 7:
			row = 2;
			col = 1;
			break;
		case 8:
			row = 2;
			col = 2;
			break;
		default:
			break;
	}
			
  for(i = 0; i < 2; i++)
  {
    
     dogs102_set_page(i+2*row+1);
    for(j=0; j<16; j++)  // cursor width = 16
    {
      dogs102_set_column(j+25+col*16);
      //dogs102_write_data(littleToad[i*16+j]);
			dogs102_write_data(0xFF);
    }
  }
}
//*****************************************************************************
void updateEnemy(int loc )
{
	enemy = loc;
}
//*****************************************************************************


void updateGame(int direction, bool action)
{
	data_rr = (tracker & 0x0F)|0xA0;
	if(action &&  (scene == 1) )
	{
		//printf("\n\r");
		if(tracker == enemy)
		{
			
			//location[tracker] = false;
			//printf("hit! ");
			endgame = 1;
		}
		else
		{

			endgame = 2;
		}
	}
	else if(direction !=4)
	{
		if(direction ==0)
		{
			switch(tracker)
			{
				case 0:
					tracker = 0;
				break;
				case 1:
					tracker = 1;
				break;
				case 2:
					tracker = 2;
				break;
				case 3:
					tracker = 0;
				break;
				case 4:
					tracker = 1;
				break;
				case 5:
					tracker = 2;
				break;
				case 6:
					tracker= 3;
				break;
				case 7:
					tracker =4;
				break;
				case 8:
					tracker = 5;
				break;
				default:
					break;
			}
		}
		else if(direction ==1)
		{
			switch(tracker)
			{
				case 0:
					tracker = 1;
				break;
				case 1:
					tracker = 2;
				break;
				case 2:
					tracker = 2;
				break;
				case 3:
					tracker = 4;
				break;
				case 4:
					tracker = 5;
				break;
				case 5:
					tracker = 5;
				break;
				case 6:
					tracker= 7;
				break;
				case 7:
					tracker =8;
				break;
				case 8:
					tracker = 8;
				break;
				default:
					break;
			}
			
		}
		else if(direction ==2)
		{
			switch(tracker)
			{
				case 0:
					tracker = 0;
				break;
				case 1:
					tracker = 0;
				break;
				case 2:
					tracker = 1;
				break;
				case 3:
					tracker = 3;
				break;
				case 4:
					tracker = 3;
				break;
				case 5:
					tracker = 4;
				break;
				case 6:
					tracker= 6;
				break;
				case 7:
					tracker =6;
				break;
				case 8:
					tracker = 7;
				break;
				default:
					break;
			}
			
		}
		else if(direction ==3)
		{
			switch(tracker)
			{
				case 0:
					tracker = 3;
				break;
				case 1:
					tracker = 4;
				break;
				case 2:
					tracker = 5;
				break;
				case 3:
					tracker = 6;
				break;
				case 4:
					tracker = 7;
				break;
				case 5:
					tracker = 8;
				break;
				case 6:
					tracker= 6;
				break;
				case 7:
					tracker =7;
				break;
				case 8:
					tracker = 8;
				break;
				default:
					break;
			}
			
		}
		
	}
	
}