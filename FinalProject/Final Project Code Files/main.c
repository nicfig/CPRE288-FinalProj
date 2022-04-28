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

typedef struct obj{
    int first;
    int second;
    int distance;
} obj;

typedef struct obj_PING{
    int angle;
    float distance;
    float width;
} obj_PING;

void scan(){
    char string[100];
    int j = 0;
    int i;
    obj objects[10];
    obj_PING toGUI[10];
    boolean isObject = false;
    servo_move(0);
    timer_waitMillis(100);
    for(i=0; i<180; i++)
    {
        int dist = ping_getDistance();
        servo_move(i);
        /* YOUR CODE HERE
        sprintf(string, "%d,\t%lf", i, ping_getDistance());

        uart_sendStr(string);
        uart_sendChar('\n');
        uart_sendChar('\r');
*/
        
        if(dist < 100){  //Get right value
            if(!isObject){
                objects[j].first = i;
                isObject = true;
            }
            else{
                objects[j].second = i;
                isObject = false;
                int mid = (objects[j].first + objects[j].second) / 2;
                toGUI[j].angle = mid;
                servo_move(mid);
                objects[j].distance = ping_getDistance();
                toGUI[j].distance = objects[j].distance;
                // Formula to find width
                j++;
            }
        }
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
/*
	char c = ' ';

	while(c != 'p'){
	    c = uart_receive();
	    if(c == 'm'){
	        scan();
	    }
	    if(c == 'w'){
	        oi_setWheels(500, 500);
	    }
	    if(c == 's'){
	        oi_setWheels(-500, -500);
	    }
        if(c == 'a'){
            oi_setWheels(500, -500);
        }
        if(c == 'd'){
            oi_setWheels(-500, 500);
        }
	}
*/
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
    return cliffStatus
}


}
