#include "servo.h"
#include "bsp_servo_pwm.h"
#include <math.h>

static float servo_angles[SERVO_COUNT];
static Servo_Config servo_configs[SERVO_COUNT];

void Servo_Init(void) {
    for (uint8_t i = 0; i < SERVO_COUNT; i++) {
        servo_angles[i] = 0.0f; 
        servo_configs[i] = (Servo_Config){
            .min_angle = 0.0f,
            .max_angle = 180.0f,
            .min_pulse = 1000,
            .max_pulse = 2000
        };   
    }
}

Servo_Status Servo_SetAngle(uint8_t servo_id, float angle)
{
    if (servo_id >= SERVO_COUNT) {
        return SERVO_INVALID_ID;
    }

    float min_angle = servo_configs[servo_id].min_angle;
    float max_angle = servo_configs[servo_id].max_angle;

    if (!isfinite(angle) || angle < min_angle || angle > max_angle) {
        return SERVO_INVALID_ANGLE;
    }

    uint16_t pulse_us = Servo_AngleToPulse(servo_id, angle);
    if (pulse_us == 0U) {
        return SERVO_CONVERSION_ERROR;
    }

    if (BSP_ServoPWM_SetPulse(servo_id, pulse_us) != BSP_SERVO_PWM_OK) {
        return SERVO_PWM_ERROR;
    }

    servo_angles[servo_id] = angle;
    return SERVO_OK;
}

float Servo_GetAngle(uint8_t servo_id)
{
    if (servo_id >= SERVO_COUNT)
    {
        return -1.0f;
    }

    return servo_angles[servo_id];
}

void Servo_Configure(uint8_t servo_id, Servo_Config config) {
    if (servo_id >= SERVO_COUNT) {
        return;
    }
    if (config.min_angle <0.0f){
        config.min_angle = 0.0f;
    }
    if (config.max_angle>180.0f){
        config.max_angle = 180.0f;
    }
    if (config.min_pulse < 1000) {
        config.min_pulse = 1000;
    }
    if (config.max_pulse > 2000) {
        config.max_pulse = 2000;
    }
    
    if (config.min_angle >= config.max_angle) {
        return;
    }

    if (config.min_pulse >= config.max_pulse) {
        return;
    }

    servo_configs[servo_id] = config;
}


uint16_t Servo_AngleToPulse(uint8_t servo_id, float angle)
{
    if (servo_id >= SERVO_COUNT) {
        return 0;
    }

    if (!isfinite(angle)) {
        return 0;
    }

    float min_angle = servo_configs[servo_id].min_angle;
    float max_angle = servo_configs[servo_id].max_angle;

    uint16_t min_pulse = servo_configs[servo_id].min_pulse;
    uint16_t max_pulse = servo_configs[servo_id].max_pulse;

    if (!isfinite(min_angle) || !isfinite(max_angle)) {
        return 0;
    }

    if (min_angle >= max_angle || min_pulse >= max_pulse) {
        return 0;
    }

    float angle_range = max_angle - min_angle;
    float pulse_range = (float)max_pulse - (float)min_pulse;

    if (angle < min_angle) {
        angle = min_angle;
    }
    else if (angle > max_angle) {
        angle = max_angle;
    }

    float pulse_width = min_pulse +
        ((angle - min_angle) / angle_range) * pulse_range;

    return (uint16_t)(pulse_width + 0.5f);
}

