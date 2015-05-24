extern "C"
{
#include <bcm2835.h>
}

#include <cstdint>

#define PIN3  RPI_V2_GPIO_P1_03
#define PIN5  RPI_V2_GPIO_P1_05
#define PIN7  RPI_V2_GPIO_P1_07
#define PIN11 RPI_V2_GPIO_P1_11
#define PIN13 RPI_V2_GPIO_P1_13
#define PIN15 RPI_V2_GPIO_P1_15
#define PIN19 RPI_V2_GPIO_P1_19
#define PIN23 RPI_V2_GPIO_P1_23
#define PIN21 RPI_V2_GPIO_P1_21

#define  GPIO_IN 0x0
#define  GPIO_OUT 0x01

#define GPIO_PULL_UP   0x02
#define GPIO_PULL_OFF  0x00
#define GPIO_PULL_DOWN 0x01

// Set output of pin, [low/high]
void GPIO_out(uint8_t pin, uint8_t state);

// Read from pin
int GPIO_read(uint8_t pin);

// Setup pin direction, [in/out]
void GPIO_setup(uint8_t pin, uint8_t dir);

void GPIO_pull(uint8_t pin, uint8_t pull);

// Set all used pins back to low
void GPIO_clean(); 
