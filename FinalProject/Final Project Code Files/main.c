/**
 * @file main.c
 * @author
 * Template file for CprE 288 Lab Final Project
 */

#include <Final Project Code Files/button.h>
#include <Final Project Code Files/lcd.h>
#include <Final Project Code Files/ping.h>
#include <Final Project Code Files/servo.h>
#include <Final Project Code Files/Timer.h>
#include "uart.h"
#include "adc.h"
#include "movement.h"
#include "open_interface.h"


// Uncomment or add any include directives that are needed

#warning "Possible unimplemented functions"
#define REPLACEME 0

void scan(){
    char string[100];
    int j;
    int i;
    servo_move(0);
    timer_waitMillis(100);
    for(i=0; i<180; i++)
    {
        servo_move(i);
        // YOUR CODE HERE
        sprintf(string, "%d,\t%lf", i, ping_getDistance());

        uart_sendStr(string);
        uart_sendChar('\n');
        uart_sendChar('\r');

        timer_waitMillis(100);
    }
}

int main(void) {
    oi_t *sensor = oi_alloc();
    oi_init(sensor);
	timer_init();
	lcd_init();
	timer_init();
	adc_init();
	servo_init();
	button_init();
	ping_init();
	uart_init();

	char c = ' ';

	while(c != 'p'){
	    c = uart_receive();
	    if(c == 'm'){
	        scan();
	    }
	    else if(c == 'w'){
	        oi_setWheels(75, 75);
	    }
	    else if(c == 's'){
	        oi_setWheels(-75, -75);
	    }
	    else if(c == 'a'){
            oi_setWheels(75, -75);
        }
	    else if(c == 'd'){
            oi_setWheels(-75, 75);
        }
	    else {
	        oi_setWheels(0, 0);
	    }
	}

    oi_free(sensor);

	return 0;

}
