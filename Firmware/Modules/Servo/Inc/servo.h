#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#define SERVO_COUNT 6U

void Servo_Init(void);
void Servo_SetAngle(uint8_t servo_id, float angle);

float Servo_GetAngle(uint8_t servo_id);

typedef struct {
    float min_angle;
    float max_angle;
    uint16_t min_pulse;
    uint16_t max_pulse;
} Servo_Config;

void Servo_Configure(uint8_t servo_id, Servo_Config config);
uint16_t Servo_AngleToPulse(uint8_t servo_id, float angle);

#endif 