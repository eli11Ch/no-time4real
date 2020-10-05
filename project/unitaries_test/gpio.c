#include "gpio.h"

int gpio_init(/*size_t pin, int direction*/) {
	return 0;
}
int gpio_write(size_t pin, char* string);
char* gpio_read(size_t pin);
int gpio_destroy(size_t pin);
