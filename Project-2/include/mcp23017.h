#ifndef __MCP23017_H__
#define __MCP23017_H__
#include <stdint.h>
#include <stdbool.h>
#include "../include/i2c.h"

#define IODIRA    0x00 
#define IODIRB    0x10 
#define IPOLA     0x01 
#define IPOLB     0x11 
#define GPINTENA  0x02 
#define GPINTENB  0x12 
#define DEFVALA   0x03 
#define DEFVALB   0x13 
#define INTCONA   0x04 
#define INTCONB   0x14 
#define IOCONA    0x05 
#define IOCONB    0x15 
#define GPPUA     0x06 
#define GPPUB     0x16 
#define INTFA     0x07 
#define INTFB     0x17 
#define INTCAPA   0x08 
#define INTCAPB   0x18 
#define GPIOPA    0x09 
#define GPIOPB    0x19 
#define OLATA     0x0A 
#define OLATB     0x1A 

i2c_status_t writeRegister(
    uint32_t  i2cBaseAddr,
    uint8_t   slaveAddr,
    uint8_t   reg,
    uint8_t   data
);

i2c_status_t readRegister(
  uint32_t  i2cBaseAddr,
  uint8_t   slaveAddr,
  uint8_t   reg,
  uint8_t   *data
);

#endif
