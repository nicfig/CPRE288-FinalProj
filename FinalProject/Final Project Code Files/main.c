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
        sprintf(string, "%d, %lf", i, ping_getDistance());

        uart_sendStr(string);

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
	int angle = 0;
	char string[100];

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
	        oi_update(sensor);
	        angle = (int)(angle + sensor->angle) % 360;
	        if(angle < 0){
                angle += 360;
	        }
	        printf("%d\n", angle);
	        sprintf(string, "%d", angle);
            uart_sendStr(string);
        }
	    else if(c == 'd'){
            oi_setWheels(-75, 75);
	        oi_update(sensor);
            angle = (int)(angle + sensor->angle) % 360;
            if(angle < 0){
                angle += 360;
            }

            printf("%d\n", angle);
            sprintf(string, "%d", angle);
            uart_sendStr(string);
        }
	    else {
	        oi_setWheels(0, 0);
	    }

	    int cCheck = cliffCheck(sensor);

	    if(cCheck == 1){
	        oi_setWheels(0, 0);
	        uart_sendStr("c1");
	    }
	    else if(cCheck == 2){
	        oi_setWheels(0, 0);
	        uart_sendStr("c2");
	    }
	    else if(cCheck == 3){
	        oi_setWheels(0, 0);
	        uart_sendStr("c3");
	    }
	    else if(cCheck == 4){
	        oi_setWheels(0, 0);
	        uart_sendStr("c4");
	    }
	}

    oi_free(sensor);

	return 0;

}

int cliffCheck(oi_t *sensor){
    oi_update(sensor);

    int cliffStatus = 0;

    if(sensor -> cliffLeft){
        cliffStatus = 1;
    }
    else if(sensor -> cliffRight){
        cliffStatus = 2;
    }
    else if(sensor -> cliffFrontLeft){
        cliffStatus = 3;
    }
    else if(sensor -> cliffFrontRight){
        cliffStatus = 4;
    }
    else{
        cliffStatus = 0;
    }
    lcd_printf("In check:  %d", cliffStatus);
    //timer_waitMillis(5000);
    return cliffStatus;
}
