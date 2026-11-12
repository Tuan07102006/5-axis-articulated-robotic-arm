#ifndef BSP_SERVO_PWM_H
#define BSP_SERVO_PWM_H

#include <stdint.h>

void BSP_ServoPWM_Start(uint8_t servo_id);
void BSP_ServoPWM_SetPulse(uint8_t servo_id, uint16_t pulse_us);
void BSP_ServoPWM_Stop(uint8_t servo_id);

#endif