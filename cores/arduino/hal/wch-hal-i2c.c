#include "Arduino.h"
#include "wch-hal-i2c.h"
#include "ch32v003fun.h"

void i2c_init(uint32_t clockSpeed, uint16_t dutyCycle, uint16_t ownAddress,
              uint16_t ack, uint16_t ackAddress) {
    // Enable I2C Pins
    RCC->APB2PCENR |= (RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO);
    RCC->APB1PCENR |= RCC_APB1Periph_I2C1;

    uint8_t pinConfig = GPIO_Speed_50MHz | GPIO_CNF_OUT_OD_AF;
    GPIOC->CFGLR &= ~(0xF << (2 * 4));
    GPIOC->CFGLR |= (pinConfig << (2 * 4));
    GPIOC->CFGLR &= ~(0xF << (1 * 4));
    GPIOC->CFGLR |= (pinConfig << (1 * 4));

    // Configure I2C peripheral

    uint16_t tmpReg, freqRange, result;
    uint32_t pClk1;

    tmpReg = I2C1->CTLR2;
    tmpReg &= CTLR2_FREQ_Reset;

    if (((RCC->CFGR0 & CFGR0_HPRE_Set_Mask) >> 4) < 8) {
        pClk1 = SYSTEM_CORE_CLOCK / ((((RCC->CFGR0 & CFGR0_HPRE_Set_Mask) >> 4) % 8) + 1);
    } else {
        pClk1 = SYSTEM_CORE_CLOCK >> ((((RCC->CFGR0 & CFGR0_HPRE_Set_Mask) >> 4) % 8) + 1);
    }

    freqRange = (uint16_t) (pClk1 / 1000000);
    tmpReg |= freqRange;
    I2C1->CTLR2 = tmpReg;

    I2C1->CTLR1 &= CTLR1_PE_Reset;
    tmpReg = 0;

    if (clockSpeed <= 100000) {
        result = (uint16_t) (pClk1 / (clockSpeed << 1));
        if (result < 0x04) {
            result = 0x04;
        }
        tmpReg |= result;
    } else {
        if (dutyCycle == I2C_DutyCycle_2) {
            result = (uint16_t) (pClk1 / (clockSpeed * 3));
        } else {
            result = (uint16_t) (pClk1 / (clockSpeed * 25));
            result |= I2C_DutyCycle_16_9;
        }

        if ((result & CKCFGR_CCR_Set) == 0) {
            result |= (uint16_t) 0x0001;
        }

        tmpReg |= (uint16_t) (result | CKCFGR_FS_Set);
    }

    I2C1->CKCFGR = tmpReg;
    I2C1->CTLR1 |= CTLR1_PE_Set;

    tmpReg = I2C1->CTLR1;
    tmpReg &= I2C_CTLR1_CLEAR_Mask;
    tmpReg |= ack;
    I2C1->CTLR1 = tmpReg;

    I2C1->OADDR1 = (ackAddress | ownAddress);

    while (i2c_get_flag_status(I2C1, I2C_FLAG_BUSY));
}

void i2c_deinit(void) {
    RCC->APB1PRSTR |= RCC_APB1Periph_I2C1;
    RCC->APB1PRSTR &= ~RCC_APB1Periph_I2C1;
}

void i2c_write_bit(uint8_t address, uint8_t data) {
    i2c_write(address, &data, 1);
}

void i2c_write(uint8_t address, uint8_t *data, uint8_t length) {
    i2c_send_start();
    while (!i2c_check_event(I2C_EVENT_MASTER_MODE_SELECT));

    i2c_send_address(address << 1, I2C_Direction_Transmitter);
    while (!i2c_check_event(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    while(length) {
        if(i2c_get_flag_status(I2C1, I2C_FLAG_TXE)) {
            i2c_send_data(*data++);
            length--;
        }
    }

    while (!i2c_check_event(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    i2c_send_stop();
}

uint8_t i2c_read_bit(uint8_t address) {
    uint8_t data = 0;
    i2c_read(address, &data, 1);
    return data;
}

void i2c_read(uint8_t address, uint8_t *data, uint8_t length) {
    i2c_send_start();

    while (!i2c_check_event(I2C_EVENT_MASTER_MODE_SELECT));

    i2c_send_address(address << 1, I2C_Direction_Receiver);

    while (!i2c_check_event(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while(length) {
        if(i2c_get_flag_status(I2C1, I2C_FLAG_RXNE)) {
            *data++ = i2c_receive_data();
            length--;
        }
    }

    i2c_send_stop();
}

void i2c_send_address(uint8_t address, uint8_t direction) {
    if (direction != I2C_Direction_Transmitter) {
        address |= OADDR1_ADD0_Set;
    } else {
        address &= OADDR1_ADD0_Reset;
    }

    I2C1->DATAR = address;
}

void i2c_send_start(void) {
    I2C1->CTLR1 |= CTLR1_START_Set;
}

void i2c_send_stop(void) {
    I2C1->CTLR1 |= CTLR1_STOP_Set;
}

bool i2c_is_init(void) {
    return true;
}

bool i2c_get_flag_status(I2C_TypeDef *I2Cx, uint32_t flag) {
    bool bitstatus = false;
    __IO uint32_t i2creg = 0, i2cxbase = 0;

    i2cxbase = (uint32_t)I2Cx;
    i2creg = flag >> 28;
    flag &= I2c_FLAG_Mask;

    if(i2creg != 0)
    {
        i2cxbase += 0x14;
    }
    else
    {
        flag = (uint32_t)(flag >> 16);
        i2cxbase += 0x18;
    }

    if(((*(__IO uint32_t *)i2cxbase) & flag) != (uint32_t)RESET)
    {
        bitstatus = true;
    }
    else
    {
        bitstatus = false;
    }

    return bitstatus;
}

bool i2c_check_event(uint32_t event) {
    uint32_t lastevent = 0;
    uint32_t flag1 = 0, flag2 = 0;
    bool status = false;

    flag1 = I2C1->STAR1;
    flag2 = I2C1->STAR2;
    flag2 = flag2 << 16;

    lastevent = (flag1 | flag2) & I2c_FLAG_Mask;

    if((lastevent & event) == event)
    {
        status = true;
    }
    else
    {
        status = false;
    }

    return status;
}

inline uint8_t i2c_receive_data() {
    return I2C1->DATAR;
}

inline void i2c_send_data(uint8_t data) {
    I2C1->DATAR = data;
}