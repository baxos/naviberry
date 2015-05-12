#include <stdint.h>
#include <stdlib.h>
#include "./bcmwrapper.h"

struct MOTOR
{
	uint8_t pin_r; 		// Pin 2A
	uint8_t pin_l;  	// Pin 1A
	uint8_t pin_e; 		// Enable pin
	uint8_t direction;	// 1 left  0 right
	uint8_t running;	// 1 on    0 off
};

struct MOTOR_PAIR
{
	struct MOTOR* motorA;
	struct MOTOR* motorB;
};


void MotorPair_Init(struct MOTOR_PAIR** motors, struct MOTOR** motorA, struct MOTOR** motorB)
{
  // Pair the two motors [A,B] together
  // To act as a side-by-side motor configutartion
  if (&(*motors) == NULL)
    {
      (*motors) = malloc(sizeof (struct MOTOR_PAIR));
    }
  else
    {
      printf("[-]Please input a NULL pointer, as motor pair structure..\n");
      exit(1);
    }

  // Now set them
  (*motors)->motorA = (*motorA);
  (*motors)->motorB = (*motorB);
}

void MotorPair_Start(struct MOTOR_PAIR** motors)
{
  // start both motors
}

void MotorPair_Stop(struct MOTOR_PAIR** motors)
{
  // stop both motors
}

void MotorPair_turnLeft(struct MOTOR_PAIR** motors)
{
  // set different directions on motors
  // turn both on
}

void MotorPair_turnRight(struct MOTOR_PAIR** motors)
{
  // set different directions on motors
  // turn both on
}

/* ================================= Motor class =========================*/

void Motor_setDirection(struct MOTOR** motor, uint8_t dir);

int Motor_Clean(struct MOTOR** motor)
{
	// Clean up
	// first set all outputs to low
	// then free memory usage
	GPIO_out((*motor)->pin_r, LOW);
	GPIO_out((*motor)->pin_l, LOW);
	GPIO_out((*motor)->pin_e, LOW);

	free((*motor));
}

int Motor_Init(struct MOTOR** motor, uint8_t pin_e, uint8_t pin_1a, uint8_t pin_2a)
{
	// Check if struct pointer is null 
	// if it is, allocate memory to it 
	// and fill it.
	if (&(*motor) == NULL)
	{
		printf("[+]Allocating memory..\n");
		(*motor) = malloc(sizeof(struct MOTOR));
	}
	printf("[+]Writing to new memory..\n");
	(*motor)->pin_r = pin_2a;
	(*motor)->pin_l = pin_1a;
	(*motor)->pin_e = pin_e;
	(*motor)->running = 0;

	// setup pins
	printf("[+]Setting pins as outputs..\n");
	GPIO_setup((*motor)->pin_r, GPIO_OUT);
	GPIO_setup((*motor)->pin_l, GPIO_OUT);
	GPIO_setup((*motor)->pin_e, GPIO_OUT);

	// set all pins low
	printf("[+]Setting all pins as low..\n");
	GPIO_out((*motor)->pin_r, LOW);
	GPIO_out((*motor)->pin_l, LOW);
	GPIO_out((*motor)->pin_e, LOW);

	printf("[+]Done setting up pins..\n");
	// setup direction
	Motor_setDirection(&(*motor), 0);
	return 0;
}

void Motor_Start(struct MOTOR** motor)
{
	// Start motor 
	// By enabling E pin
	GPIO_out((*motor)->pin_e, HIGH);
	(*motor)->running = 1;
}

void Motor_Stop(struct MOTOR** motor)
{
	// Stop motor
	// By disabling E pin
	GPIO_out((*motor)->pin_e, LOW);
	(*motor)->running = 0;
}

void Motor_setDirection(struct MOTOR** motor, uint8_t dir)
{
	if ((*motor) == NULL)
	{
		// Error
	}
	// Set local direction
	uint8_t currentState = (*motor)->running;

	// halt motor
	if (currentState != 0)
		Motor_Stop(&(*motor));

	// select direction
	switch(dir)
	{
		case 0: // right
		GPIO_out((*motor)->pin_l, LOW);
		GPIO_out((*motor)->pin_r, HIGH);
		break;
		case 1: //left
		GPIO_out((*motor)->pin_r, LOW);
		GPIO_out((*motor)->pin_l, HIGH);
		break;
	}

	// Save  direction
	(*motor)->direction = dir;

	// Put motor back in state
	if (currentState != 0)
		Motor_Start(&(*motor));

	// Done
}

