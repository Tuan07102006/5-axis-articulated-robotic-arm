#include "bsp_servo_pwm.h"
#include "tim.h"

typedef struct {
    TIM_HandleTypeDef *timer;
    uint32_t channel;
} ServoPWM_Map;

static const ServoPWM_Map servo_pwm_map[6] = {
    {&htim3, TIM_CHANNEL_1},
    {&htim3, TIM_CHANNEL_2},
    {&htim3, TIM_CHANNEL_3},
    {&htim3, TIM_CHANNEL_4},
    {&htim4, TIM_CHANNEL_1},
    {&htim4, TIM_CHANNEL_2}
};

void BSP_ServoPWM_Start(uint8_t servo_id) {

    if (servo_id >= 6U) {
        return;
    }

    HAL_TIM_PWM_Start(servo_pwm_map[servo_id].timer, servo_pwm_map[servo_id].channel);
}

void BSP_ServoPWM_SetPulse(uint8_t servo_id, uint16_t pulse_us) {

    if (servo_id >= 6U) {
        return;
    }

    if (pulse_us < 1000 || pulse_us > 2000) {
        return;
    }

    __HAL_TIM_SET_COMPARE(servo_pwm_map[servo_id].timer, servo_pwm_map[servo_id].channel, pulse_us);
}

void BSP_ServoPWM_Stop(uint8_t servo_id) {

    if (servo_id >= 6U) {
        return;
    }

    HAL_TIM_PWM_Stop(servo_pwm_map[servo_id].timer, servo_pwm_map[servo_id].channel);
}