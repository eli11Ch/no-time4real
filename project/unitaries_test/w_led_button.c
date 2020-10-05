#include <wiringPi.h>

#define BUTTON 1 // GPIO 18
#define LED 24 // GPIO 19

int main(void) {
	//init
	wiringPiSetup();
	pinMode(LED, OUTPUT);
	pinMode(BUTTON, INPUT);
	
	while(1) { //loop
		if(digitalRead(BUTTON)) {
			digitalWrite(LED, 0);
		} else {
			digitalWrite(LED, 1);
		}
	}
	return 0;
}
