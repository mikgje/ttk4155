#include "servo.h"
#include "pwm.h"

void adc_receive(CanMsg* rx_message) {
    can_rx(rx_message);
    //printf("Joystick X: %d, Joystick Y: %d\n\rSlider Left: %d, Slider Right: %d\n\rButton Left: %d, Button Right: %d\n\r",
    //rx_message->byte8.bytes[1], rx_message->byte8.bytes[0], rx_message->byte8.bytes[2], rx_message->byte8.bytes[3], rx_message->byte8.bytes[4], rx_message->byte8.bytes[5]);
}

void servo_control(CanMsg* rx_message) {
    uint32_t joystick_x, duty_cycle;
    adc_receive(rx_message);
    joystick_x = rx_message->byte8.bytes[1];
    duty_cycle = 900 + joystick_x * ((2100 - 900)/255);
    printf("Duty cycle: %d", duty_cycle);
    pwm_set_duty_cycle(duty_cycle);
}