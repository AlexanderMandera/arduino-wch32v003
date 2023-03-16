#include "ch32v003fun.h"
#include "HardwareSerial.h"

HardwareSerial Serial;

#define RX_BUFFER_SIZE 100
u8 rxBuffer[RX_BUFFER_SIZE] = {0};
u8 rxBufferHead = 0;
u8 rxBufferTail = 0;

void HardwareSerial::begin(unsigned long baud, uint16_t config) {
    // Hardware Serial Pins D5 / D6
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1; // Enable UART
    GPIOD->CFGLR &= ~(0xf<<(4*5));
    GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*5);

    USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Tx;
    USART1->CTLR2 = USART_StopBits_1;
    USART1->CTLR3 = USART_HardwareFlowControl_None;

    USART1->BRR = UART_BRR;
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
}

size_t HardwareSerial::write(uint8_t c) {
    USART1->DATAR = (uint16_t)(c & (uint16_t)0x01FF);
    while((USART1->STATR & USART_FLAG_TXE) == RESET);
    return 1;
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size) {
    for(int i = 0; i < size; i++) {
        while( !(USART1->STATR & USART_FLAG_TC));
        USART1->DATAR = *buffer++;
    }
    return size;
}

HardwareSerial::operator bool() {
    return true;
}

/*void USART1_IRQHandler() {
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
}*/