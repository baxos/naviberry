#include <bcm2835.h>
#include <stdio.h>
#include "./bcmwrapper.h"
#include "./motordriver.h"

int main()
{
	if (!bcm2835_init())
		return 1;

	// Setup pins as outout
	GPIO_setup(PIN23,GPIO_OUT);
	GPIO_setup(PIN21, GPIO_OUT); 

	char counter = 5;

	// Blink
	while (counter > 0)
	{
		printf("[+]Run %d \n", counter);
		printf("\t [+]LED 1\n");
		GPIO_out(PIN23, HIGH);
		GPIO_out(PIN21, LOW);
		delay(1000);


		printf("\t [+]LED 2\n");
		GPIO_out(PIN23, LOW);
		GPIO_out(PIN21, HIGH);
		delay(1000);

		counter--;
	}	


	GPIO_out(PIN21, LOW);
	GPIO_out(PIN23, LOW);
	printf("[+]Exit..\n");
	return 0;
}
