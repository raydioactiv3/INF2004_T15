#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

int main() {
    stdio_init_all();

    const uint8_t addr = 0x1E;  // GY-511 sensor's I2C address

    i2c_init(i2c0, 100000);  // Initialize I2C with a 400 kHz baud rate
    gpio_set_function(4, GPIO_FUNC_I2C);  // GPIO 2 as SDA
    gpio_set_function(5, GPIO_FUNC_I2C);  // GPIO 3 as SCL
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    uint8_t config[] = {0x02, 0x00};  // Register address 0x00 and mode 0x70 (8 samples @ 15Hz)
    i2c_write_blocking(i2c0, addr, config, sizeof(config), false);

    while(1){
        uint8_t out_x = 0x03;
        uint8_t buffer[6];
        i2c_write_blocking(i2c0, addr, &out_x, 1, true);
        i2c_read_blocking(i2c0, addr, buffer, sizeof(buffer), false);

        // Parse the data (little-endian format)
        int16_t x = ((buffer[0] << 8) | (buffer[1]));
        int16_t z = ((buffer[2] << 8) | (buffer[3]));
        int16_t y = ((buffer[4] << 8) | (buffer[5]));

        printf("Magnetometer Data: X=%d, Y=%d, Z=%d\n", x, y, z);

        sleep_ms(1000);
    }

    return 0;
}