#include "hal/wch-hal-i2c.h"

void setup() {
    // Initialize Serial Port
    Serial.begin(115200);

    // Initialize I2C
    i2c_init(80000, I2C_DutyCycle_16_9, 0x02, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
}

uint8_t read_data[10] = {0};

void loop() {
    delay(1000);

    Serial.println("Trying to read 6 bytes from 0x30");

    // Read 6 bits from slave
    i2c_read(0x30, read_data, 6);

    Serial.println("Done reading");

    Serial.print("Recv: ");
    for(uint8_t d = 0; d < 6; d++) {
        Serial.print(read_data[d]);
    }
    Serial.println();
}
