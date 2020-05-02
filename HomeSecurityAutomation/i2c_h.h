#ifndef _I2C_H_
#define _I2C_H_
#include"types.h"
extern void i2cStart(void);
extern void i2cStop(void);
extern void i2cWrite(u8);
extern u8 i2cRead(void);
extern void i2cAck(void);
extern void i2cMasterAck(void);
extern void i2cNoAck(void);
extern void i2cRestart(void);
extern void i2cDevWrite(u8,u8,u8);
extern u8 i2cDevRead(u8,u8);
							
#endif