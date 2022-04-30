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
	char string[100];
	int j;
	int i;
	servo_move(0);
	double err = ping_getDistance();
	timer_waitMillis(300);
	for (i = 0; i <= 180; i++)
	{
		servo_move(i);
		// YOUR CODE HERE
		sprintf(string, "%d %lf", i, ping_getDistance());

		uart_sendStr(string);

		timer_waitMillis(100);
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

	char c = ' ';
	int angle = 0;
	double dist = 0;
	char string[100];

	while (c != 'p')
	{
		c = uart_receive();
        oi_update(sensor);
		if (c == 'm')
		{
			scan();
		}
		else if (c == 'w')
		{
			oi_setWheels(75, 75);
			dist = dist + sensor->distance;
			sprintf(string, "%lf", dist);
			uart_sendStr(string);
		}
		else if (c == 's')
		{
			oi_setWheels(-75, -75);
            dist = dist + sensor->distance;
            sprintf(string, "%lf", dist);
            uart_sendStr(string);
		}
		else if (c == 'a')
		{
			oi_setWheels(75, -75);
			angle = (int)(angle + sensor->angle) % 360;
			if (angle < 0)
			{
				angle += 360;
			}
			printf("%d\n", angle);
			sprintf(string, "%d", angle);
			uart_sendStr(string);
		}
		else if (c == 'd')
		{
			oi_setWheels(-75, 75);
			angle = (int)(angle + sensor->angle) % 360;
			if (angle < 0)
			{
				angle += 360;
			}

			printf("%d\n", angle);
			sprintf(string, "%d", angle);
			uart_sendStr(string);
		}
		else
		{
			oi_setWheels(0, 0);
//	        sprintf(string, " ");
//	        uart_sendStr(string);
		}


	}

	oi_free(sensor);

	return 0;
}
