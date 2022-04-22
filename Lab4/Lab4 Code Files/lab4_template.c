/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include <Lab4 Code Files/button.h>
#include <Lab4 Code Files/cyBot_uart.h>  // Functions for communicating between CyBot and Putty (via UART)
#include <Lab4 Code Files/lcd.h>
#include <Lab4 Code Files/Timer.h>
#include "string.h"

#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	            // Don't forget to initialize the cyBot UART before trying to use it

	// YOUR CODE HERE
	cyBot_uart_init();
	
	while(1)
	{

      // YOUR CODE HERE
	    uint8_t data = button_getButton();
	    char msg1[] = "button 1\n\r";
	    char msg2[] = "button 2\n\r";
	    char msg3[] = "button 3\n\r";
	    char msg4[] = "button 4\n\r";
	    int i = 0;

	    if(data == 0b00001000){
	        lcd_printf("4");
	        for(i = 0; i < strlen(msg4); i++){
	            cyBot_sendByte(msg4[i]);
	        }
	        timer_waitMillis(300);

	    }
	    else if(data == 0b00000100){
	        lcd_printf("3");
            for(i = 0; i < strlen(msg3); i++){
                cyBot_sendByte(msg3[i]);
            }
            timer_waitMillis(300);
	    }
	    else if(data == 0b00000010){
	        lcd_printf("2");
            for(i = 0; i < strlen(msg2); i++){
                cyBot_sendByte(msg2[i]);
            }
            timer_waitMillis(300);
	    }
        else if(data == 0b00000001){
            lcd_printf("1");
            for(i = 0; i < strlen(msg2); i++){
                cyBot_sendByte(msg1[i]);
            }
            timer_waitMillis(300);
        }


	}

}
