#include "EEPROM_24C.h"

// EEPROM I2C address (7-bit address)
#define EEPROM_ADDRESS  	0x50  // A2 A1 A0 = 0
#define EEPROM_TOTAL_SIZE	32768
#define EEPROM_PAGE_SIZE	64
#define EEPROM_PAGE_COUNT	512

void EEPROM_Init(void)
{
	// Initialize USER LED
	LED_Init();
	// Initialize UART Peripheral
	UART2_Init();
	// Initialize I2C Peripheral
	I2C1_Init();
	// Send message to Terminal
	UART2_TxString("[info] EEPROM Initialized.\n\r");
}

void EEPROM_WriteByte(uint16_t memAddress, uint8_t data)
{
    // Generate start condition
    I2C1_Start();
    // Send EEPROM's device address (0xA0 for write operation, assuming A2/A1/A0 = 0)
    I2C1_SendAddress(EEPROM_ADDRESS, 0);  // 7-bit address
    // Send the high byte of the memory address
    I2C1_SendData((memAddress >> 8) & 0xFF);
    // Send the low byte of the memory address
    I2C1_SendData(memAddress & 0xFF);
    // Send the data byte to be written
    I2C1_SendData(data);
    // Send message to Terminal
    UART2_TxString("[info] Byte send to EEPROM.\n\r");
    // Generate stop condition
    I2C1_Stop();
}

void EEPROM_WritePage(uint16_t memAddress, uint8_t* data, uint16_t length)
{
    // Calculate the number of bytes left in the current page
    uint16_t pageOffset = memAddress % EEPROM_PAGE_SIZE;  	// Find the current position within the page
    uint16_t pageRemaining = EEPROM_PAGE_SIZE - pageOffset;  	// Number of bytes left in the page

    // If the data length exceeds the remaining space in the page, trim it
    if (length > pageRemaining)
    {
        length = pageRemaining;  // Limit the write to the end of the page
    }
    // Generate start condition
    I2C1_Start();
    // Send EEPROM's device address (0xA0 for write operation, assuming A2/A1/A0 = 0)
    I2C1_SendAddress(EEPROM_ADDRESS, 0);  // 7-bit address
    // Send the high byte of the memory address
    I2C1_SendData((memAddress >> 8) & 0xFF);
    // Send the low byte of the memory address
    I2C1_SendData(memAddress & 0xFF);
    // Send data until the end of the page or until the specified length
    for (uint16_t i = 0; i < length; i++)
    {
        I2C1_SendData(data[i]);
    }
    // Generate stop condition
    I2C1_Stop();
}

void EEPROM_WriteBlock(uint16_t memAddress, uint8_t* data, uint16_t totalLength)
{
    uint16_t bytesWritten = 0;  // Track the number of bytes written so far
    // Continue writing until all data is written
    while (totalLength > 0)
    {
        // Calculate the current page's starting address and remaining bytes in the block
        uint16_t pageOffset = memAddress % EEPROM_PAGE_SIZE;  // Find position in the page
        uint16_t pageRemaining = EEPROM_PAGE_SIZE - pageOffset; // Bytes remaining in the current page
        // Determine how much data can be written in the current page
        uint16_t bytesToWrite = (totalLength < pageRemaining) ? totalLength : pageRemaining;
        // Write the data for the current page
        EEPROM_WritePage(memAddress, data + bytesWritten, bytesToWrite);
        // Update address, bytes written, and remaining length
        memAddress += bytesToWrite;
        bytesWritten += bytesToWrite;
        totalLength -= bytesToWrite;
        // Optional: Add a delay to wait for EEPROM write cycle completion
        delay_ms(25);  // Ensure this function adds the necessary delay
    }
    // Send message to Terminal
    UART2_TxString("[info] Block stored in EEPROM.\n\r");
}

uint8_t EEPROM_ReadByte_Current(void)
{
	// Declare an 8-bit variable to hold received data
	uint8_t rx_byte;
	// Generate start condition
	I2C1_Start();
	// Send EEPROM's Device Address with Read Mode
	I2C1_SendAddress(EEPROM_ADDRESS, 1);
	// Read data from current memory address
	rx_byte = I2C1_ReadData();
	// Generate Stop Condition
	I2C1_Stop();
	// Return the received data
	return rx_byte;
}

uint8_t EEPROM_ReadByte_Random(uint16_t memAddress)
{
	uint8_t rx_byte;
	I2C1_Start();
	I2C1_SendAddress(EEPROM_ADDRESS, 0);
	I2C1_SendData((memAddress>>8) & 0xFF);
	I2C1_SendData((memAddress) & 0xFF);
	I2C1_Start();
	I2C1_SendAddress(EEPROM_ADDRESS, 1);
	I2C1_DisableACK();
	I2C1_Stop();
	rx_byte = I2C1_ReadData();
	return rx_byte;
}

void EEPROM_ReadPage(uint16_t memAddress, uint8_t *data, uint16_t length)
{
    I2C1_Start();
    I2C1_SendAddress(EEPROM_ADDRESS, 0);
    I2C1_SendData((memAddress>>8) & 0xFF);
    I2C1_SendData((memAddress) & 0xFF);
    I2C1_Start();
    I2C1_SendAddress(EEPROM_ADDRESS, 1);

    while(length > 0)
    {
    	if(length == 1)
    	{
    		*data++ = I2C1_ReadData();
    		I2C1_DisableACK();
    		I2C1_Stop();
    		break;
    	}
    	else
    	{
    		I2C1_EnableACK();
    		(*data++) = I2C1_ReadData();
    		length--;
    	}
    }
}

void EEPROM_ReadBlock(uint16_t memAddress, uint8_t *data, uint16_t length)
{

}

void EEPROM_EraseByte(uint16_t memAddress)
{
	EEPROM_WriteByte(memAddress, 0xFF);
}

void EEPROM_ErasePage(uint16_t memAddress)
{
	uint8_t buf[64];
	for(int i = 0; i < 64; i++)
	{
		buf[i] = 0xFF;
	}
	EEPROM_WritePage(memAddress, buf, 64);
}

void EEPROM_EraseBlock(void)
{

}


