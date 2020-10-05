#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/gpio.h>

#define RPI_GPIO_IN 18 // Button
#define RPI_GPIO_OUT 19 // Led

int main(void /*int argc, char** argv*/) {
	// init
	gpio_request(RPI_GPIO_IN, "gpio18");	
	gpio_direction_input(RPI_GPIO_IN);	
	gpio_request(RPI_GPIO_OUT, "gpio19");
	gpio_direction_output(RPI_GPIO_IN, 0);	

	while(1) { // boucle
		if(gpio_get_value(RPI_GPIO_IN)) {
			gpio_set_value(RPI_GPIO_OUT,0);
		} else {
			gpio_set_value(RPI_GPIO_OUT,1);
		}
	}
	return 0;
}
