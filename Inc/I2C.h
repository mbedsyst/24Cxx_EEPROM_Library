#ifndef I2C_H_
#define I2C_H_

#include <stdbool.h>
#include "stm32f4xx.h"

void I2C1_Init(void);
void I2C1_Start(void);
void I2C1_SendAddress(uint8_t address, bool mode);
void I2C1_SendData(uint8_t data);
uint8_t I2C1_ReadData(void);
void I2C1_EnableACK(void);
void I2C1_DisableACK(void);
void I2C1_Stop(void);

#endif
