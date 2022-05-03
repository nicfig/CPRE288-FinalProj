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
	int i;
	servo_move(0);
	double err = ping_getDistance();
	timer_waitMillis(500);
	for (i = 0; i < 180; i++)
	{
		servo_move(i);
		double sumDist = 0;
		int j;
		for (j = 0; j < scanAmt; j++)
		{
			if (adc_read() > 750)
			{
				sumDist += ping_getDistance();
			}
			timer_waitMillis(50);
		}
		double dist;
		if (sumDist > 0)
		{
			dist = sumDist / scanAmt;
		}
		else
		{
			dist = 999;
		}

		sprintf(string, "m %d %lf", i, dist);

		uart_sendStr(string);
	}
}

int cliffCheck(oi_t *sensor)
{
	oi_update(sensor);

	int cliffStatus = 0;

	if (sensor->cliffLeft)
	{
		cliffStatus = 4;
	}
	else if (sensor->cliffRight)
	{
		cliffStatus = 1;
	}
	else if (sensor->cliffFrontLeft)
	{
		cliffStatus = 3;
	}
	else if (sensor->cliffFrontRight)
	{
		cliffStatus = 2;
	}
	else
	{
		cliffStatus = 0;
	}
	lcd_printf("In check:  %d", cliffStatus);
	// timer_waitMillis(5000);
	return cliffStatus;
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
	bool obs = false;

	while (1)
	{
		int cCheck = cliffCheck(sensor);
		int leftLine = sensor->cliffLeftSignal;
		int frontLeftLine = sensor->cliffFrontLeftSignal;
		int rightLine = sensor->cliffRightSignal;
		int frontRightLine = sensor->cliffFrontRightSignal;
		obs = false;

		c = uart_receive();
		if (cCheck == 1)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("c1");
		}
		if (cCheck == 2)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("c2");
		}
		if (cCheck == 3)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("c3");
		}
		if (cCheck == 4)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("c4");
		}

		if (leftLine > 2700)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("v1");
		}
		if (frontLeftLine > 2700)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("v2");
		}
		if (frontRightLine > 2700)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("v3");
		}
		if (rightLine > 2700)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("v4");
		}

		if (sensor->bumpLeft)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("b1");
		}
		if (sensor->bumpRight)
		{
			obs = true;
			oi_setWheels(0, 0);
			uart_sendStr("b2");
		}

		if (c == 'm')
		{
			c = uart_receive();
			scan();
		}
		else if ((c == 'w') && (obs == false))
		{
			oi_setWheels(75, 75);
			uart_sendStr("w");
		}
		else if ((c == 's'))
		{
			oi_setWheels(-75, -75);
			uart_sendStr("s");
		}
		else if ((c == 'a'))
		{
			oi_setWheels(75, -75);
			uart_sendStr("a");
		}
		else if ((c == 'd'))
		{
			oi_setWheels(-75, 75);
			uart_sendStr("d");
		}
		else if (c == 'p')
		{
			break;
		}
		else
		{
			oi_setWheels(0, 0);
		}
		oi_update(sensor);
	}

	oi_free(sensor);

	return 0;
}
