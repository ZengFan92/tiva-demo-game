#include "boardUtil.h"

void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

/*
#define UART0_BUFFER_SIZE 80
PC_Buffer UART0_Rx_Buffer;
char UART0_Rx_Buffer_Array[UART0_BUFFER_SIZE];

PC_Buffer UART0_Tx_Buffer;
char UART0_Tx_Buffer_Array[UART0_BUFFER_SIZE];
*/
//*****************************************************************************
// Configure PA0 and PA1 to be UART pins
//*****************************************************************************
void uart0_config_gpio(void)
{
   gpio_enable_port(GPIOA_BASE);
   gpio_config_digital_enable( GPIOA_BASE, PA0 | PA1);
   gpio_config_alternate_function( GPIOA_BASE, PA0 | PA1);
   gpio_config_port_control( GPIOA_BASE, GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX);
}

//*****************************************************************************
//*****************************************************************************
void serialDebugInit(void)
{
  // Configure GPIO Pins
  uart0_config_gpio();
  

	uart0_init_115K();
}

//*****************************************************************************
// Initialize the I2C peripheral
//*****************************************************************************
void i2cInit(void)
{
  
  gpio_enable_port(EEPROM_GPIO_BASE);
  
  // Configure SCL 
  gpio_config_digital_enable(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PIN);
  gpio_config_alternate_function(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PIN);
  gpio_config_port_control(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PIN_PCTL);

  
  // Configure SDA 
  gpio_config_digital_enable(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
  gpio_config_open_drain(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
  gpio_config_alternate_function(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
  gpio_config_port_control(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN_PCTL);
  
  //  Initialize the I2C peripheral
  initializeI2CMaster(EEPROM_I2C_BASE);
	
  
}
//*****************************************************************************
// Initialize the LCD peripheral
//*****************************************************************************

void lcd_init(void)
{
  
  // Configure SPI CLK
  gpio_enable_port(LCD_GPIO_BASE);
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_CLK_PIN_PCTL);
    
  // Configure SPI CS
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_CS_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_CS_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_CS_PIN_PCTL);

  // Configure SPI MOSI
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_MOSI_PIN_PCTL);
  
  // Configure CD
  gpio_enable_port(GPIO_LCD_CD_BASE);
  gpio_config_digital_enable(GPIO_LCD_CD_BASE,LCD_CD_PIN);
  gpio_config_enable_output(GPIO_LCD_CD_BASE,LCD_CD_PIN);
  
  // Configure RST_N
  gpio_enable_port(GPIO_LCD_RST_N_BASE);
  gpio_config_digital_enable(GPIO_LCD_RST_N_BASE, LCD_RST_N_PIN);
  gpio_config_enable_output(GPIO_LCD_RST_N_BASE, LCD_RST_N_PIN);
	//CommentsLCD
  // Configure the SPI interface for Mode 3, 25MHz.
  // Make sure to adjust the registers used to determine the frequency of 
  // the SPI clock.
  initialize_spi( LCD_SPI_BASE, 3);
  
}


//*****************************************************************************
//*****************************************************************************

void rfInit(void)
{  
	GPIOA_Type  *gpioPort;

  wireless_set_pin_config(
    RF_SPI_BASE,
    RF_PAYLOAD_SIZE,
    RF_CHANNEL,
    RF_CS_BASE,
    RF_CS_PIN,
    RF_CE_GPIO_BASE,
    RF_CE_PIN
  );
	gpioPort = (GPIOA_Type *)RF_IRQ_GPIO_BASE;
  gpio_enable_port(RF_GPIO_BASE);
  
  // Configure SPI CLK
  gpio_config_digital_enable(  RF_GPIO_BASE, RF_CLK_PIN);
  gpio_config_alternate_function(    RF_GPIO_BASE, RF_CLK_PIN);
  gpio_config_port_control(     RF_GPIO_BASE, RF_CLK_PIN_PCTL);
  
  // // CommentsWireless:Configure SPI MISO, input enable, detect interrupts 
  gpio_config_digital_enable(  RF_GPIO_BASE, RF_MISO_PIN);
  gpio_config_alternate_function(    RF_GPIO_BASE, RF_MISO_PIN);
  gpio_config_port_control(     RF_GPIO_BASE, RF_MISO_PIN_PCTL);
	gpio_config_enable_input(    RF_GPIO_BASE, RF_MISO_PIN_PCTL);
	// enable interrupts
	gpio_enable_port(RF_IRQ_GPIO_BASE);
	gpio_config_digital_enable(RF_IRQ_GPIO_BASE,RF_IRQ_PIN);
	//CommentsWireless
	gpio_config_enable_input(    RF_IRQ_GPIO_BASE, RF_IRQ_PIN);
	gpio_enable_interrupts(    RF_IRQ_GPIO_BASE, RF_IRQ_PIN);
	NVIC_SetPriority(GPIOD_IRQn, 1);
  NVIC_EnableIRQ(GPIOD_IRQn);
	
  // Configure SPI MOSI
  gpio_config_digital_enable(  RF_GPIO_BASE, RF_MOSI_PIN);
  gpio_config_alternate_function(    RF_GPIO_BASE, RF_MOSI_PIN);
  gpio_config_port_control(     RF_GPIO_BASE, RF_MOSI_PIN_PCTL);
  //()RF_SPI_BASE->IM = 1<<2;
	

  // Configure CS to be a normal GPIO pin that is controlled 
  // explicitly by software
    gpio_enable_port(RF_CS_BASE);
  gpio_config_digital_enable(  RF_CS_BASE,RF_CS_PIN);
  gpio_config_enable_output(    RF_CS_BASE,RF_CS_PIN);
  
  // Configure CE Pin as an output
  
  gpio_enable_port(RF_CE_GPIO_BASE);
  gpio_config_digital_enable(  RF_CE_GPIO_BASE,RF_CE_PIN);
  gpio_config_enable_output(    RF_CE_GPIO_BASE,RF_CE_PIN);

  
  initialize_spi( RF_SPI_BASE, 0);
  RF_CE_PERIH->DATA |= (1 << 1);
  

}

//*****************************************************************************
// Set new wireless IDs
//*****************************************************************************

void setWirelessIDs(uint8_t *myID, uint8_t *remoteID)
{
  
  char msg[2];
  
  printf("Enter two byte hexidecimal number for 353 Controller: 0x353");
  scanf("%s",msg);
  myID[0] = '3';
  myID[1] = '5';
  myID[2] = '3';
  myID[3] = msg[0];
  myID[4] = msg[1];

  eeprom_byte_write(EEPROM_I2C_BASE,0, myID[0]);
  eeprom_byte_write(EEPROM_I2C_BASE,1, myID[1]);
  eeprom_byte_write(EEPROM_I2C_BASE,2, myID[2]);
  eeprom_byte_write(EEPROM_I2C_BASE,3, myID[3]);
  eeprom_byte_write(EEPROM_I2C_BASE,4, myID[4]);

  printf("Enter two byte hexidecimal number for the 315 Robot: 0x353");
  scanf("%s",msg);
  remoteID[0] = '3';
  remoteID[1] = '5';
  remoteID[2] = '3';
  remoteID[3] = msg[0];
  remoteID[4] = msg[1];
  eeprom_byte_write(EEPROM_I2C_BASE,5, remoteID[0]);
  eeprom_byte_write(EEPROM_I2C_BASE,6, remoteID[1]);
  eeprom_byte_write(EEPROM_I2C_BASE,7, remoteID[2]);
  eeprom_byte_write(EEPROM_I2C_BASE,8, remoteID[3]);
  eeprom_byte_write(EEPROM_I2C_BASE,9, remoteID[4]);
  wireless_configure_device(myID, remoteID ) ;
  
  printf("STAFF   ID: %s\n\r", myID);
  printf("STUDENT ID: %s\n\r", remoteID);
  
}
  
//*****************************************************************************
// Read in and set the wirless IDs
//*****************************************************************************

void getWirelessIDs(uint8_t *myID, uint8_t *remoteID)
{
  char msg[2];
  // Read in the device IDs
  eeprom_byte_read(EEPROM_I2C_BASE,0, &myID[0]);
  eeprom_byte_read(EEPROM_I2C_BASE,1, &myID[1]);
  eeprom_byte_read(EEPROM_I2C_BASE,2, &myID[2]);
  eeprom_byte_read(EEPROM_I2C_BASE,3, &myID[3]);
  eeprom_byte_read(EEPROM_I2C_BASE,4, &myID[4]);
  
  if( myID[0] != '3' ||  myID[1] != '5' || myID[2] != '3')
  {
    printf("Enter two byte hexidecimal number for this board: 0x353");
    scanf("%s",msg);
    myID[0] = '3';
    myID[1] = '5';
    myID[2] = '3';
    myID[3] = msg[0];
    myID[4] = msg[1];

    eeprom_byte_write(EEPROM_I2C_BASE,0, myID[0]);
    eeprom_byte_write(EEPROM_I2C_BASE,1, myID[1]);
    eeprom_byte_write(EEPROM_I2C_BASE,2, myID[2]);
    eeprom_byte_write(EEPROM_I2C_BASE,3, myID[3]);
    eeprom_byte_write(EEPROM_I2C_BASE,4, myID[4]);
  }
  
  eeprom_byte_read(EEPROM_I2C_BASE,5, &remoteID[0]);
  eeprom_byte_read(EEPROM_I2C_BASE,6, &remoteID[1]);
  eeprom_byte_read(EEPROM_I2C_BASE,7, &remoteID[2]);
  eeprom_byte_read(EEPROM_I2C_BASE,8, &remoteID[3]);
  eeprom_byte_read(EEPROM_I2C_BASE,9, &remoteID[4]);
  
  if( remoteID[0] != '3' ||  remoteID[1] != '5' || remoteID[2] != '3')
  {
    printf("Enter two byte hexidecimal number for remote board: 0x353");
    scanf("%s",msg);
    remoteID[0] = '3';
    remoteID[1] = '5';
    remoteID[2] = '3';
    remoteID[3] = msg[0];
    remoteID[4] = msg[1];
    eeprom_byte_write(EEPROM_I2C_BASE,5, remoteID[0]);
    eeprom_byte_write(EEPROM_I2C_BASE,6, remoteID[1]);
    eeprom_byte_write(EEPROM_I2C_BASE,7, remoteID[2]);
    eeprom_byte_write(EEPROM_I2C_BASE,8, remoteID[3]);
    eeprom_byte_write(EEPROM_I2C_BASE,9, remoteID[4]);
  }
  //wireless_configure_device(myID, remoteID ) ;
  printf("STAFF   ID: %s\n\r", myID);
  printf("STUDENT ID: %s\n\r", remoteID);
  
}


