#ifndef GPIO_H
#define GPIO_H

#include <stdlib.h>

int gpio_init(size_t pin, int direction);
int gpio_write(size_t pin, char* string);
char* gpio_read(size_t pin);
int gpio_destroy(size_t pin);
#endif //GPIO_H
