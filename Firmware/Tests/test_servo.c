#include <assert.h>
#include <stdio.h>

#include "servo.h"
#include "bsp_servo_pwm.h"

static unsigned pwm_calls;
static uint8_t last_servo_id;
static uint16_t last_pulse_us;
static BSP_ServoPWM_Status pwm_result = BSP_SERVO_PWM_OK;

BSP_ServoPWM_Status BSP_ServoPWM_SetPulse(uint8_t servo_id, uint16_t pulse_us)
{
    ++pwm_calls;
    last_servo_id = servo_id;
    last_pulse_us = pulse_us;
    return pwm_result;
}

int main(void)
{
    Servo_Init();

    Servo_Status status = Servo_SetAngle(0U, 90.0f);
    assert(status == SERVO_OK);
    assert(pwm_calls == 1U);
    assert(last_servo_id == 0U);
    assert(last_pulse_us == 1500U);
    assert(Servo_GetAngle(0U) == 90.0f);

    status = Servo_SetAngle(0U, 200.0f);
    assert(status == SERVO_INVALID_ANGLE);
    assert(pwm_calls == 1U);

    pwm_result = BSP_SERVO_PWM_INVALID_PULSE;
    status = Servo_SetAngle(0U, 100.0f);
    assert(status == SERVO_PWM_ERROR);
    assert(pwm_calls == 2U);
    assert(Servo_GetAngle(0U) == 90.0f);

    puts("Servo tests passed");
    return 0;
}