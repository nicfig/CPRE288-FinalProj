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

void scan()
{
    int scanAmt = 3;
	char string[100];
	int j;
	int i;
	servo_move(0);
	double err = ping_getDistance();
	timer_waitMillis(500);
	for (i = 0; i < 180; i++)
	{
	    servo_move(i);
	    double sumDist = 0;
        int j;
        for(j = 0; j < scanAmt; j++){
            if(adc_read() > 750){
                sumDist += ping_getDistance();
            }
            timer_waitMillis(50);
        }
        double dist;
        if(sumDist > 0){
            dist = sumDist / scanAmt;
        }
        else{
            dist = 999;
        }

        sprintf(string, "m %d %lf", i, dist);


		uart_sendStr(string);

	}
}

int main(void)
{
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
	adc_init();

	char c = ' ';
	int angle = 0;
	double dist = 0;
	char string[100];

	while (1)
	{
		c = uart_receive();
		if (c == 'm')
		{
			scan();
		}
		else if (c == 'w')
		{
			oi_setWheels(75, 75);
			uart_sendStr("w");
		}
		else if (c == 's')
		{
			oi_setWheels(-75, -75);
			uart_sendStr("s");

		}
		else if (c == 'a')
		{
			oi_setWheels(75, -75);
			uart_sendStr("a");
		}
		else if (c == 'd')
		{
			oi_setWheels(-75, 75);
			uart_sendStr("d");
		}
		else if(c == 'p'){
		    break;
		}
		else
		{
			oi_setWheels(0, 0);
		}
		oi_update(sensor);
//        dist = dist + sensor->distance;
//        angle = (int)(angle + sensor->angle) % 360;
//        if (angle < 0)
//        {
//            angle += 360;
//        }
//        sprintf(string, "d %lf %d", dist, angle);
	}

	oi_free(sensor);

	return 0;
}
