#include "ch32v003fun.h"
#include "HardwareSerial.h"

HardwareSerial Serial;

#define RX_BUFFER_SIZE 100
u8 rxBuffer[RX_BUFFER_SIZE] = {0};
u8 rxBufferHead = 0;
u8 rxBufferTail = 0;

void HardwareSerial::begin(unsigned long baud, uint16_t config) {
    // Hardware Serial Pins D5 / D6

    RCC->APB2PCENR |= RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOD; // Enable UART
    GPIOD->CFGLR &= ~(0xf<<(4*5)); // Clear D5
    GPIOD->CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_OUT_PP_AF)<<(4*5); // Set D5 to Push-Pull
    GPIOD->CFGLR &= ~(0xf<<(4*6)); // Clear D6
    GPIOD->CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_IN_FLOATING)<<(4*6); // Set D6 to Floating

    USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Tx | USART_Mode_Rx;
    USART1->CTLR2 = USART_StopBits_1;
    USART1->CTLR3 = USART_HardwareFlowControl_None;

    // Set baud rate
    //uint32_t integerDivider = ((25 * APB_CLOCK)) / (OVER8DIV * baud);
    //uint32_t fractionalDivider = integerDivider % 100;

    //USART1->BRR = ((integerDivider / 100) << 4) | (((fractionalDivider * (OVER8DIV * 2) + 50) / 100) & 7);
    USART1->BRR = UART_BRR;

    // Enable peripheral
    USART1->CTLR1 |= CTLR1_UE_Set;
}

void HardwareSerial::begin(unsigned long baud) {
    begin(baud, 0); // TODO: Add config options
}

void HardwareSerial::end() {
    USART1->CTLR1 &= CTLR1_UE_Reset;
}

int HardwareSerial::available() {
    return rxBufferTail - rxBufferHead;
}

int HardwareSerial::peek() {
    return rxBuffer[rxBufferHead];
}

int HardwareSerial::read() {
    return rxBuffer[rxBufferHead++];
}

void HardwareSerial::flush() {
    while(!(USART1->STATR & USART_STATR_TXE));
}

size_t HardwareSerial::write(uint8_t c) {
    while( !(USART1->STATR & USART_FLAG_TC));
    USART1->DATAR = ((uint16_t)c & (uint16_t)0x01FF);
    return 1;
}

HardwareSerial::operator bool() {
    return available();
}

void USART1_IRQHandler() {
    if(USART1->STATR & USART_FLAG_RXNE) {
        // Write into buffer
        rxBufferTail = (rxBufferTail + 1) % RX_BUFFER_SIZE;

        if(rxBufferTail == rxBufferHead) {
            // Buffer empty, disable USART interrupt
            USART1->CTLR1 &= ~USART_FLAG_RXNE;
            return;
        }

        rxBuffer[rxBufferTail++] = USART1->DATAR & (uint16_t)0x01FF;
    }
}