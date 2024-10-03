#ifndef EEPROM_24C_H_
#define EEPROM_24C_H_

#include "LED.h"
#include "I2C.h"
#include "SYSTICK.h"
#include "UART.h"

void EEPROM_Init(void);
void EEPROM_WriteByte(uint16_t memAddress, uint8_t data);
void EEPROM_WritePage(uint16_t memAddress, uint8_t* data, uint16_t length);
void EEPROM_WriteBlock(uint16_t memAddress, uint8_t* data, uint16_t length);
uint8_t EEPROM_ReadByte_Current(void);
uint8_t EEPROM_ReadByte_Random(uint16_t memAddress);
void EEPROM_ReadPage(uint16_t memAddress, uint8_t *data, uint16_t length);
void EEPROM_ReadBlock(uint16_t memAddress, uint8_t *data, uint16_t length);
void EEPROM_EraseByte(uint16_t memAddress);
void EEPROM_ErasePage(uint16_t memAddress);
void EEPROM_EraseBlock(void);

#endif
