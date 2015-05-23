// Copyright (c) 2014-15, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "lcd.h"
#include "../src/boardUtil.h"
#include "fonts.h"


const uint8_t image[] =
{
// page1, 52cols
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0xF0,0xF0,0xF0,0xF0,  0xF0,0xF0,0xF0,0xF0,  0xFF,0xFF,0xFF,0xFF,  0x0F,0x0F,0x0F,0x0F,  0x0F,0x0F,0x0F,0x0F,  
0x0F,0x0F,0x0F,0x0F,  0x0F,0x0F,0x0F,0x0F,  0xFF,0xFF,0xFF,0xFF,  0xF0,0xF0,0xF0,0xF0,  0xF0,0xF0,0xF0,0xF0,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  
// page2, 
0xF0,0xF0,0xF0,0xF0,  0xFF,0xFF,0xFF,0xFF,  0x0F,0x0F,0x0F,0x0F,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x0F,0x0F,0x0F,0x0F,  0xFF,0xFF,0xFF,0xFF,  0xF0,0xF0,0xF0,0xF0,
// page3,
0xFF,0xFF,0xFF,0xFF,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0xFF,0xFF,0xFF,0xFF,
// page4,
0xFF,0xFF,0xFF,0xFF,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0xFF,0xFF,0xFF,0xFF,
// page5,
0xFF,0xFF,0xFF,0xFF,  0xF0,0xF0,0xF0,0xF0,  0xF0,0xF0,0xF0,0xF0,  0xFF,0xFF,0xFF,0xFF,  0x0F,0x0F,0x0F,0x0F,  0xFF,0xFF,0xFF,0xFF,  0x0F,0x0F,0x0F,0x0F,
0x0F,0x0F,0x0F,0x0F,  0xFF,0xFF,0xFF,0xFF,  0x0F,0x0F,0x0F,0x0F,  0xFF,0xFF,0xFF,0xFF,  0xF0,0xF0,0xF0,0xF0,  0xF0,0xF0,0xF0,0xF0,  0xFF,0xFF,0xFF,0xFF,
// page6,
0x00,0x00,0x00,0x00,  0x0F,0x0F,0x0F,0x0F,  0xFF,0xFF,0xFF,0xFF,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x0F,0x0F,0x0F,0x0F,  0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,  0x0F,0x0F,0x0F,0x0F,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0xFF,0xFF,0xFF,0xFF,  0x0F,0x0F,0x0F,0x0F,  0x00,0x00,0x00,0x00,
// page7,
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x0F,0x0F,0x0F,0x0F,  0xFF,0xFF,0xFF,0xFF,  0xF0,0xF0,0xF0,0xF0,  0xF0,0xF0,0xF0,0xF0,  0xF0,0xF0,0xF0,0xF0,
0xF0,0xF0,0xF0,0xF0,  0xF0,0xF0,0xF0,0xF0,  0xF0,0xF0,0xF0,0xF0,  0xFF,0xFF,0xFF,0xFF,  0x0F,0x0F,0x0F,0x0F,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00  
};


const uint8_t ccWidthPixels = 37;
const uint8_t ccHeightPixels = 45;



//*****************************************************************************
// Use the GPIO port connected to the LCD Reset line to hold the 
// LCD in reset.  The signal is active low
// 
// ADD CODE
//*****************************************************************************
__INLINE static void dogs102_assert_reset(void)
{

	GPIOC->DATA &= ~(1<<7);

}

//*****************************************************************************
// Use the GPIO port connected to the LCD Reset line to bring the 
// LCD out of reset by setting the pin high
// 
// ADD CODE
//*****************************************************************************
__INLINE static void dogs102_deassert_reset(void)
{
	 GPIOC->DATA |= (1<<7);
 
}

//*****************************************************************************
// The LCD can be put into command mode by writing a 0 to the CMD pin.  
// In command mode, you can set the active page or page.
// 
// ADD CODE
//*****************************************************************************
__INLINE static void dogs102_assert_cmd_mode(void)
	
{
	 GPIOA->DATA &= ~(1<<4);

}

//*****************************************************************************
// Turn OFF command mode by writeing a 1 to the CMD pin
//
// When the LCD is not in command command mode, any data that is written over
// the SPI interface is treated as data to turn ON/OFF pixels at the location
// indicated by the currently active page and column.
// 
// ADD CODE
//*****************************************************************************
__INLINE static void dogs102_deassert_cmd_mode(void)
{
	GPIOA->DATA |= (1<<4);

}

//*****************************************************************************
// Set the page address to page 7-0.  The orientation of the 353 carrier card
// puts page 0 at the top of the display and page 7 at the bottom.
//
// See "Set Page Address" in the EADOGS102W-6 data sheet, pg 5.
//
// Make sure to set the command mode correctly!
// 
// ADD CODE
//*****************************************************************************
void dogs102_set_page(uint8_t   page)
{
	  uint8_t tx_data;
  uint8_t rx_data;
    //Enter Command Mode
	dogs102_assert_cmd_mode();
    
    // Set the active page using spiTx
	tx_data = 0xB0 | (page);
	spiTx(SSI0_BASE,&tx_data,1, &rx_data);
    
    //Exit Command Mode
	dogs102_deassert_cmd_mode();

}

//*****************************************************************************
// There are 102 columns in the display.  Use this function to set which colum
// data will be written to.
//
// See "Set Column Address LSB and MSB" in the EADOGS102W-6 data sheet, pg 5.
// This will require two different SPI transactions.
//
// Make sure to set the command mode correctly!
  // 
// ADD CODE
//*****************************************************************************
void dogs102_set_column(uint8_t   column)
{
	  uint8_t tx_data;
  uint8_t rx_data;

  //Enter Command Mode
  dogs102_assert_cmd_mode();
  // Set the active column LSB using spiTx
		tx_data = 0x00| (column & 0x0F);
	spiTx(SSI0_BASE,&tx_data,1, &rx_data);
                  
  // Set the active column MSB using spiTx
		tx_data = 0x10|((column & 0xF0) >>4);
	spiTx(SSI0_BASE,&tx_data,1, &rx_data);
  
  //Exit Command Mode
	dogs102_deassert_cmd_mode();

}

//*****************************************************************************
// When not in command mode, any data written to the LCD is used to determine
// which pixels are turned ON/OFF for the curretnly active page.  A 1 turns a 
// pixel on and a 0 turns the pixel off.
// 
// ADD CODE
//*****************************************************************************
void dogs102_write_data( uint8_t   data)
{
	  uint8_t tx_data;
  uint8_t rx_data;
  
  //Exit Command Mode
	dogs102_deassert_cmd_mode();
  // Send the data using spiTx
	tx_data = data;
	spiTx(SSI0_BASE,&tx_data,1, &rx_data);


}


  
  
  
  
  
//*****************************************************************************
// PROVIDED CODE
//*****************************************************************************  

//*****************************************************************************
// Supports writing a 10 point character to the LCD.  There are "4" lines on 
// the LCD,
//*****************************************************************************
 void dogs102_write_char_10pts( uint8_t line, char c, uint8_t charIndex)
  {
    
    uint16_t i, index;
    uint8_t upperPage, lowerPage;
    
    switch (line)
    {
      case 0:
      {
        upperPage = 0;
        lowerPage = 1;
        break;
      }
      
       case 1:
      {
        upperPage = 2;
        lowerPage = 3;
        break;
      }
      
       case 2:
      {
        upperPage = 4;
        lowerPage = 5;
        break;
      }
      
      case 3:
      {
        upperPage = 6;
        lowerPage = 7;
        break;
      }
    }
    if(c >= 32 || c <= 126)
    {
      
      index = c - 32;
      index = index * 20;
      
      dogs102_set_page(upperPage) ;
      
      for( i=0; i< 10; i++)
      {
        dogs102_set_column(charIndex*10 + i);
        dogs102_write_data(courierNew_10ptBitmaps[index+i]);
      }
      
      dogs102_set_page(lowerPage) ;
      
      for( i=10; i< 20; i++)
      {
        dogs102_set_column(charIndex*10 + (i-10));
        dogs102_write_data(courierNew_10ptBitmaps[index+i]);
      }

    }

  }

//*****************************************************************************
// Used to clear the LCD of all pixels
//*****************************************************************************
void dogs102_clear(void)
{
  uint8_t page;
  uint8_t colCount;
  
  for(page = 0; page < 8; page++)
  {
      dogs102_set_page(page) ;

      for( colCount=0; colCount< 102; colCount++)
      {
        dogs102_set_column(colCount);
        dogs102_write_data(0x00);
      }
    }
}

//*****************************************************************************
// Provided initialization code for the LCD.  Found in the data sheet.
//*****************************************************************************
void dogs102_init(void)
{
  uint8_t tx_data;
  uint8_t rx_data;

  
  //Take the LCD out of reset
  dogs102_deassert_reset();
  
  //Enter Command Mode
  dogs102_assert_cmd_mode();
  
 //Set Scroll Line
  tx_data = 0x40;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //Set SEG Directions
  tx_data = 0xA1;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //Set COM direction
  tx_data = 0xC0;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //Set All Pixel on
  tx_data = 0xA4;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //Set Inverse Display
  tx_data = 0xA6;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //et LCD Bias Ratio
  tx_data = 0xA2;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //Set Power Control
  tx_data = 0x2F;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //Set VLCD Resistor Ratio
  tx_data = 0x27;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //Set Electronic Volume
  tx_data = 0x81;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  tx_data = 0x10;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
  //Set Adv Program Control
  tx_data = 0xFA;
  spiTx(SSI0_BASE, &tx_data, 1, &rx_data);
  tx_data = 0x90;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);
  
 //Set Display Enable
  tx_data = 0xAF;
  spiTx(SSI0_BASE,&tx_data, 1, &rx_data);

  //Exit Command Mode
  dogs102_deassert_cmd_mode();
  
}

