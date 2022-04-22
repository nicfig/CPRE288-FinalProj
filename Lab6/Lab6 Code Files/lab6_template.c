/**
 * lab6_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include <Lab6 Code Files/cyBot_Scan.h>  // For scan sensors
#include <Lab6 Code Files/lcd.h>
#include <Lab6 Code Files/Timer.h>
#include <Lab6 Code Files/uart.h>

// Uncomment or add any include directives that are needed
// #include "open_interface.h"
// #include "movement.h"
// #include "button.h"


//#warning "Possible unimplemented functions"
#define REPLACEME 0


//int main(void) {
//	timer_init(); // Must be called before lcd_init(), which uses timer functions
//	lcd_init();
//	uart_init();
//
//	// YOUR CODE HERE
//    cyBOT_init_Scan(0b0111);
//    right_calibration_value = 285259;
//    left_calibration_value = 1272250;
//
//    cyBOT_Scan_t scan;
//
//    cyBOT_Scan(0, &scan);
//    timer_waitMillis(300);
//
//    int i = 0;
//
//	while(1)
//	{
//
//      // YOUR CODE HERE
//	  if(uart_receive() == 'g'){
//	      while(1){
//	          if(uart_receive_nonblocking() == 's'){
//	              break;
//	          }
//
//	          else {
//	              if(i > 180){
//	                  i = 0;
//	              }
//	              cyBOT_Scan(i, &scan);
//	              i++;
//	          }
//	      }
//
//	  }
//
//	}
//
//}
