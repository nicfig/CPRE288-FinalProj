/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include <Lab10 Code Files/cyBot_uart.h>
#include <Lab10 Code Files/lcd.h>
#include <Lab10 Code Files/ping.h>
#include <Lab10 Code Files/Timer.h>
#include <Lab10 Code Files/servo.h>
#include <Lab10 Code Files/button.h>


// Uncomment or add any include directives that are needed

#warning "Possible unimplemented functions"
#define REPLACEME 0

int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	servo_init();
	button_init();


//	ping_init();
//	cyBot_uart_init();
//	// YOUR CODE HERE
//    char string[100];
//    int j;
//	while(1)
//	{
//
//      // YOUR CODE HERE
//        sprintf(string, "%lf", ping_getDistance());
//
//
//        for(j=0; j < strlen(string); j++){
//            cyBot_sendByte(string[j]);
//        }
//        cyBot_sendByte('\n');
//        cyBot_sendByte('\r');
//
//	    timer_waitMillis(100);
//
//
//	}




	servo_move(90);
	timer_waitMillis(500);
	servo_move(0);
	timer_waitMillis(500);
	servo_move(180);
	timer_waitMillis(500);
	servo_move(90);

    timer_waitMillis(500);
    servo_move(180);

	while(1) {
	    button_move();
	}

	return 0;

}
