#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include "./bcmwrapper.h"
#include "./motordriver.h"

int main()
{
	if (!bcm2835_init())
		return 1;

	struct MOTOR* motorA = malloc(sizeof(struct MOTOR*));
	struct MOTOR* motorB = NULL;
	

	// Turn on connected LED
	GPIO_setup(PIN5, GPIO_OUT);
	GPIO_out(PIN5, HIGH);

	printf("[+]Waiting for start signal..\n");


	printf("[+]Initializing motor..\n");

	Motor_Init(&motorA, PIN11, PIN13, PIN15);
	delay(5000);


	printf("[+]Starting motor..\n");
	Motor_Start(&motorA);

	delay(5000);

	printf("[+]Switch direction..\n");
	Motor_setDirection(&motorA, 1);

	printf("[+] Stopping motor..\n");
	Motor_Stop(&motorA);


	delay(5000);
	printf("[+]Clean motor class\n");


	Motor_Clean(&motorA);

	// Turn off connected LED
	GPIO_out(PIN5, LOW);


	return 0;
}
