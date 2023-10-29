#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"

#define IR_SENSOR_LEFT_PIN  14  // Pin for the left IR sensor
#define IR_SENSOR_RIGHT_PIN 15  // Pin for the right IR sensor
#define MOTOR_PIN1 16           // Pin for motor control
#define MOTOR_PIN2 17

// Function to initialize the IR sensors
void initIRSensors() {
    gpio_init(IR_SENSOR_LEFT_PIN);
    gpio_init(IR_SENSOR_RIGHT_PIN);
    gpio_set_dir(IR_SENSOR_LEFT_PIN, GPIO_IN);
    gpio_set_dir(IR_SENSOR_RIGHT_PIN, GPIO_IN);
}

// Function to initialize the motor control pins
void initMotor() {
    gpio_init(MOTOR_PIN1);
    gpio_init(MOTOR_PIN2);
    gpio_set_dir(MOTOR_PIN1, GPIO_OUT);
    gpio_set_dir(MOTOR_PIN2, GPIO_OUT);
}

// Function for line following
void followLine() {
    while (1) {
        bool left_sensor = gpio_get(IR_SENSOR_LEFT_PIN);
        bool right_sensor = gpio_get(IR_SENSOR_RIGHT_PIN);

        if (left_sensor && right_sensor) {
            // Both sensors on the line, move forward
            gpio_put(MOTOR_PIN1, 1);
            gpio_put(MOTOR_PIN2, 0);
            printf("Move Forward!\n");
            sleep_ms(1000);
        } else if (left_sensor) {
            // Only left sensor is on the line, turn left
            gpio_put(MOTOR_PIN1, 0);
            gpio_put(MOTOR_PIN2, 0);
            printf("Turn Left!\n");
            sleep_ms(1000);
        } else if (right_sensor) {
            // Only right sensor is on the line, turn right
            gpio_put(MOTOR_PIN1, 1);
            gpio_put(MOTOR_PIN2, 1);
            printf("Turn Right!\n");
            sleep_ms(1000);
        } else {
            // Both sensors are off the line, stop or apply some other logic
            // You may need additional code to determine what to do when off the line
        }
    }
}

int main() {
    stdio_init_all();
    initIRSensors();
    initMotor();

    while (1) {
        followLine();
    }

    return 0;
}
