#ifndef _WCH_HAL_I2C_H
#define _WCH_HAL_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ch32v003fun.h"
#include "stdbool.h"

void i2c_init(uint32_t clockSpeed, uint16_t dutyCycle, uint16_t ownAddress, uint16_t ack, uint16_t ackAddress);
void i2c_deinit(void);
void i2c_write_bit(uint8_t address, uint8_t data);
void i2c_write(uint8_t address, uint8_t *data, uint8_t length);
uint8_t i2c_read_bit(uint8_t address);
void i2c_read(uint8_t address, uint8_t *data, uint8_t length);
void i2c_send_address(uint8_t address, uint8_t direction);
void i2c_send_start(void);
void i2c_send_stop(void);
bool i2c_is_init(void);

bool i2c_get_flag_status(I2C_TypeDef *I2Cx, uint32_t flag);
bool i2c_check_event(uint32_t event);
inline uint8_t i2c_receive_data();
inline void i2c_send_data(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif // _WCH_HAL_I2C_H_
