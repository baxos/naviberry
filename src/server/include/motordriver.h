#include <stdint.h>

struct MOTOR
{
	uint8_t pin_r;  // Pin 2A
	uint8_t pin_l;  // Pin 1A
	uint8_t pin_e; 	// Enable pin
	uint8_t direction;
	uint8_t running;
};

int Motor_Init(struct MOTOR** motor, uint8_t pin_e, uint8_t pin_1a, uint8_t pin_2a);
void Motor_Start(struct MOTOR** motor);
void Motor_Stop(struct MOTOR** motor);
void Motor_setDirection(struct MOTOR** motor, uint8_t dir);

