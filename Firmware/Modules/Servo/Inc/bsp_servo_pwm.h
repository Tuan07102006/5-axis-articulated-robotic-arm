#ifndef BSP_SERVO_PWM_H
#define BSP_SERVO_PWM_H

#include <stdint.h>

typedef enum {
    BSP_SERVO_PWM_OK = 0,
    BSP_SERVO_PWM_INVALID_ID,
    BSP_SERVO_PWM_INVALID_PULSE,
    BSP_SERVO_PWM_HAL_ERROR
} BSP_ServoPWM_Status;

BSP_ServoPWM_Status BSP_ServoPWM_Start(uint8_t servo_id);
BSP_ServoPWM_Status BSP_ServoPWM_SetPulse(uint8_t servo_id, uint16_t pulse_us);
BSP_ServoPWM_Status BSP_ServoPWM_Stop(uint8_t servo_id);

#endif