#include "bsp_servo_pwm.h"
#include "tim.h"

#define PWM_CHANNEL_COUNT (sizeof(servo_pwm_map) / sizeof(servo_pwm_map[0]))

typedef struct {
    TIM_HandleTypeDef *timer;
    uint32_t channel;
} ServoPWM_Map;

static const ServoPWM_Map servo_pwm_map[] = {
    {&htim3, TIM_CHANNEL_1},
    {&htim3, TIM_CHANNEL_2},
    {&htim3, TIM_CHANNEL_3},
    {&htim3, TIM_CHANNEL_4},
    {&htim4, TIM_CHANNEL_1},
    {&htim4, TIM_CHANNEL_2}
};

BSP_ServoPWM_Status BSP_ServoPWM_Start(uint8_t servo_id)
{
    if (servo_id >= PWM_CHANNEL_COUNT) {
        return BSP_SERVO_PWM_INVALID_ID;
    }

    const ServoPWM_Map *map = &servo_pwm_map[servo_id];

    if (HAL_TIM_PWM_Start(map->timer, map->channel) != HAL_OK) {
        return BSP_SERVO_PWM_HAL_ERROR;
    }

    return BSP_SERVO_PWM_OK;
}

BSP_ServoPWM_Status BSP_ServoPWM_SetPulse(uint8_t servo_id, uint16_t pulse_us)
{
    if (servo_id >= PWM_CHANNEL_COUNT) {
        return BSP_SERVO_PWM_INVALID_ID;
    }

    if (pulse_us < 1000U || pulse_us > 2000U) {
        return BSP_SERVO_PWM_INVALID_PULSE;
    }

    const ServoPWM_Map *map = &servo_pwm_map[servo_id];
    __HAL_TIM_SET_COMPARE(map->timer, map->channel, pulse_us);

    return BSP_SERVO_PWM_OK;
}

BSP_ServoPWM_Status BSP_ServoPWM_Stop(uint8_t servo_id)
{
    if (servo_id >= PWM_CHANNEL_COUNT) {
        return BSP_SERVO_PWM_INVALID_ID;
    }

    const ServoPWM_Map *map = &servo_pwm_map[servo_id];

    if (HAL_TIM_PWM_Stop(map->timer, map->channel) != HAL_OK) {
        return BSP_SERVO_PWM_HAL_ERROR;
    }

    return BSP_SERVO_PWM_OK;
}