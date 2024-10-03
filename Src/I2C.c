#include "I2C.h"

void I2C1_Init(void)
{
    // Enable the I2C1 and GPIOB clocks
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;  // Enable I2C1 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock
    // Configure PB8 (SCL) and PB9 (SDA) for I2C alternate function
    GPIOB->MODER &= ~((3U << 2 * 8) | (3U << 2 * 9));  // Clear bits
    GPIOB->MODER |= (2U << 2 * 8) | (2U << 2 * 9);     // Set to AF mode (10)
    GPIOB->AFR[1] |= (4U << 4 * (8 - 8)) | (4U << 4 * (9 - 8));  // Set AF4 (I2C) for PB8 and PB9
    GPIOB->OTYPER |= (1U << 8) | (1U << 9);            // Open-drain for I2C
    GPIOB->OSPEEDR |= (3U << 2 * 8) | (3U << 2 * 9);   // High speed
    GPIOB->PUPDR |= (1U << 2 * 8) | (1U << 2 * 9);     // Pull-up resistors
    // Reset I2C1 to clear any previous settings
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    // Set the I2C clock frequency (APB1 is running at 42MHz)
    I2C1->CR2 = 42;  // 42 MHz APB1 clock
    // Configure clock control for 400kHz I2C operation
    I2C1->CCR = 0x50;  // CCR = 80, Fast mode, 400kHz (see reference manual for calculations)
    I2C1->TRISE = 43;  // Maximum rise time
    // Enable I2C1
    I2C1->CR1 |= I2C_CR1_PE;
}

// Function to generate a start condition on I2C1
void I2C1_Start(void)
{
    I2C1->CR1 |= I2C_CR1_START;  // Generate start condition
    while (!(I2C1->SR1 & I2C_SR1_SB));  // Wait for start condition generated
}

// Function to send the EEPROM address and wait for acknowledgment
void I2C1_SendAddress(uint8_t address, bool mode)
{
	if(mode) // Execute in READ Mode
	{
		// Send the 7-bit address with the read bit
		I2C1->DR = ((address << 1) | 0x01);
	}
	else     // Execute in WRITE Mode
	{
		// Send the 7-bit address with the write bit
		I2C1->DR = (address << 1);
	}
	// Wait for address acknowledgment
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    // Clear the ADDR flag by reading SR2
    (void)I2C1->SR2;
}

// Function to send a byte of data on I2C1
void I2C1_SendData(uint8_t data)
{
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // Wait for data register empty
    I2C1->DR = data;  // Load data into the data register
}

void I2C1_EnableACK(void)
{
	//Disable acknowledgment
	I2C1->CR1 |= I2C_CR1_ACK;
	// Clear SR2
	(void)I2C1->SR2;
}

void I2C1_DisableACK(void)
{
	//Disable acknowledgment
	I2C1->CR1 &= ~I2C_CR1_ACK;
	// Clear SR2
	(void)I2C1->SR2;
}

// Function to receive a byte of data on I2C1
uint8_t I2C1_ReadData(void)
{
	// Wait for data register to be filled
	while (!(I2C1->SR1 & I2C_SR1_RXNE));
	// Load data into the variable
    uint8_t data = I2C1->DR;
    // Return the 8-bit Data
    return data;
}

// Function to generate a stop condition on I2C1
void I2C1_Stop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;  // Generate stop condition
}
