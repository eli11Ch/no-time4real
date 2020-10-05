#include <wiringPi.h>
#include <softPwm.h>

#define SERVO1 21 // BCM 5
#define SERVO2 27 // BCM 16

#define MIN1 0
#define MAX1 100

#define MIN2 0
#define MAX2 100

#define TIME 100

int main(void) {
	// init
	wiringPiSetup();
	softPwmCreate(SERVO1, MIN1, MAX1);
	softPwmCreate(SERVO2, MIN2, MAX2);
	while(1) { // loop
		for(int i = MIN1; i<=MAX1; i++){
			softPwmWrite(SERVO1, i);
			softPwmWrite(SERVO2, i);
			delay(TIME);
		}	
		for(int i = MAX1; i>= MIN1; i--){
			softPwmWrite(SERVO1, i);
			softPwmWrite(SERVO2, i);
			delay(TIME);
		}	
	}
	return 0;
}
