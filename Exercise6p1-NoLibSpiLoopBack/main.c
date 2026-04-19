#include <stdint.h>

// Base Addresses
#define RCC_BASE      0x40021000
#define SPI1_BASE     0x40013000
#define GPIOA_BASE    0x40010800
// Registers
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define SPI1_CR1      (*(volatile uint32_t *)(SPI1_BASE + 0x00))
#define GPIOA_CRL     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
// Status Register Bit Definitions
#define SPI1_DR      (*(volatile uint32_t *)(SPI1_BASE + 0x0C))
#define SPI1_SR      (*(volatile uint32_t *)(SPI1_BASE + 0x08))
#define SPI_SR_TXE   (1 << 1)  // Transmit buffer empty
#define SPI_SR_RXNE  (1 << 0)  // Receive buffer not empty
#define SPI_SR_BSY   (1 << 7)  // Busy flag

void SPI1_GPIO_Init(void) {
    // 1. Enable GPIOA Clock (Bit 2 in APB2ENR)
    RCC_APB2ENR |= (1 << 2);

    // 2. Configure PA5, PA6, PA7 using GPIOA_CRL (Pins 0-7)
    // Each pin uses 4 bits. PA5 = bits 20-23, PA6 = bits 24-27, PA7 = bits 28-31.
    
    // Clear the configuration bits for pins 5, 6, and 7
    GPIOA_CRL &= ~((0xF << 20) | (0xF << 24) | (0xF << 28));

    // --- PA5: SCK ---
    // Mode: Output 50MHz (11), CNF: Alt Function Push-Pull (10) -> 1011 (0xB)
    GPIOA_CRL |= (0xB << 20);

    // --- PA6: MISO ---
    // Mode: Input (00), CNF: Floating Input (01) -> 0100 (0x4)
    GPIOA_CRL |= (0x4 << 24);

    // --- PA7: MOSI ---
    // Mode: Output 50MHz (11), CNF: Alt Function Push-Pull (10) -> 1011 (0xB)
    GPIOA_CRL |= (0xB << 28);
}

void SPI1_Master_Init(void) {
    // 0. Enable SPI1 Clock (Bit 12 in APB2ENR)
    RCC_APB2ENR |= (1 << 12);

    /* --- Procedure from RM0041 Section 21.3.3 --- */

    // 1. Select the BR[2:0] bits (Baud Rate)
    // Clear the bits first (bits 5, 4, and 3)
    SPI1_CR1 &= ~(0x7 << 3); 
    // Set fPCLK/64. Binary 101 = 0x5.
    SPI1_CR1 |= (0x5 << 3);

    // 2. Select CPOL and CPHA (Clock Polarity/Phase)
    // Setting CPOL=0, CPHA=0 (Mode 0)
    SPI1_CR1 &= ~(1 << 1); // CPOL = 0
    SPI1_CR1 &= ~(1 << 0); // CPHA = 0

    // 3. Set the DFF bit (Data Frame Format)
    // 0: 8-bit, 1: 16-bit. Let's use 8-bit.
    SPI1_CR1 &= ~(1 << 11); 

    // 4. Configure LSBFIRST bit
    // 0: MSB transmitted first, 1: LSB transmitted first. Use MSB.
    SPI1_CR1 &= ~(1 << 7);

    // 5. NSS (Slave Select) Management
    // We will use "Software Mode" (SSM=1, SSI=1). 
    // This tricks the SPI into thinking NSS is high so it stays in Master mode.
    SPI1_CR1 |= (1 << 9); // SSM = 1 (Software Slave Management)
    SPI1_CR1 |= (1 << 8); // SSI = 1 (Internal Slave Select)

    // 6. Set MSTR and SPE bits
    SPI1_CR1 |= (1 << 2); // MSTR = 1 (Master Mode)
    SPI1_CR1 |= (1 << 6); // SPE = 1 (SPI Enable)
}

/**
 * @brief Transmits one byte and returns the byte received simultaneously.
 * Follows RM0041 Section: Full-duplex transmit and receive procedure.
 */
uint8_t SPI1_TransmitReceive(uint8_t data) {
    // 1. Wait until TXE=1 (Ensure Tx buffer is empty)
    while (!(SPI1_SR & SPI_SR_TXE));

    // 2. Write data to SPI_DR (This clears TXE and starts the clock)
    SPI1_DR = data;

    // 3. Wait until RXNE=1 (Wait for the shift register to fill back up)
    while (!(SPI1_SR & SPI_SR_RXNE));

    // 4. Read SPI_DR (This returns the received data and clears RXNE)
    return (uint8_t)SPI1_DR;
}

void SPI1_SendBuffer(uint8_t* tx_data, uint8_t* rx_data, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        // Exchange data
        uint8_t received = SPI1_TransmitReceive(tx_data[i]);
        
        // If the user provided an RX buffer, store the result
        if (rx_data != 0) {
            rx_data[i] = received;
        }
    }

    // 5. Finalize: Wait until TXE=1 and then wait until BSY=0
    while (!(SPI1_SR & SPI_SR_TXE));
    while (SPI1_SR & SPI_SR_BSY);
}

int main(void) {
    uint8_t txbuf[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t rxbuf[4];

    // Initialize (Using your previous functions)
    SPI1_GPIO_Init();
    SPI1_Master_Init();

    while(1) {
        // Send 4 bytes and receive 4 bytes
        SPI1_SendBuffer(txbuf, rxbuf, 4);
        
        // Add some delay or logic here
    }
}