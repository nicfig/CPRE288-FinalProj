/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
//#include "stdio.h"
#include "string.h"

int main (void) {
    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                      // and enables time functions (e.g. timer_waitMillis)

    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    char text[] = {"                    Microcontrollers are fun!"};
    char arr[21];
    int length = strlen(text);

    int i,j;

    while(1) {
        for (i = 0; i < length; i++) {
            text[length] = text[0];
            for (j = 0; j < length; j++) {
                text[j] = text[j+1];
            }
            strncpy(arr,text,20);
            timer_waitMillis(300);
            lcd_printf(arr);
        }

    }
    return 0;
}



