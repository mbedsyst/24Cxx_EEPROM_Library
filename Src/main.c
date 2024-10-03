#include "stdio.h"
#include "EEPROM_24C.h"

int main()
{
	// uint8_t data[64] = "The STM32F401RET6 Nucleo-64 Development Board is cheap and easy!";
	uint8_t buf[64];
	EEPROM_Init();
	LED_Toggle();
	EEPROM_WriteByte(0x0000, 'Z');
	EEPROM_ReadPage(0x0000, buf, 64);
	LED_Toggle();

	for(int i = 0; i < 64; i++)
	{
		UART2_TxChar(buf[i]);
	}

	while(1)
	{

	}

}
