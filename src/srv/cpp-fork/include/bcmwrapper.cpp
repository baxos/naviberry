extern "C"
{
#include "./bcm2835.h"
}
#include <stdio.h>
#include <stdint.h>


void GPIO_out(uint8_t pin, uint8_t state)
{
	bcm2835_gpio_write(pin, state);
}

int GPIO_read(uint8_t pin)
{
  return bcm2835_gpio_lev(pin);
}

void GPIO_setup(uint8_t pin, uint8_t dir)
{
	bcm2835_gpio_fsel(pin, dir);
}

void GPIO_pull(uint8_t pin, uint8_t pull)
{
  bcm2835_gpio_set_pud(pin, pull);
}
