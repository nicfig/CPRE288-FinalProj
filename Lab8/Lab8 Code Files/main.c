/**
 * lab6_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include <Lab8 Code Files/cyBot_Scan.h>
#include <Lab8 Code Files/cyBot_uart.h>
#include <Lab8 Code Files/movement.h>
#include <Lab8 Code Files/open_interface.h>
#include <Lab8 Code Files/open_interface.h>
#include <Lab8 Code Files/Timer.h>
#include <Lab8 Code Files/uart.h>
#include "string.h"
#include "uart.h"
#include "adc.h"
#include <math.h>

// Uncomment or add any include directives that are needed
// #include "button.h"


//#warning "Possible unimplemented functions"
#define REPLACEME 0

int j=0;

int main(void) {
       oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
       oi_init(sensor_data); // do this only once at start of main()

       timer_init();
       lcd_init();
       lcd_clear();


       cyBOT_init_Scan(0b0111);
       //cyBOT_SERVO_cal();
       uart_init();
       adc_init();
       right_calibration_value = 253750;
       left_calibration_value = 1235500;


       cyBOT_Scan_t scan;


       char string[100];

       cyBOT_Scan(90, &scan);
       timer_waitMillis(300);
//
//       while(1){
//           cyBOT_Scan(90, &scan);
//           sprintf(string, "%lf", scan.sound_dist);
//
//            //Send info
//            for(j=0; j < strlen(string); j++){
//                cyBot_sendByte(string[j]);
//            }
//            cyBot_sendByte('\n');
//            cyBot_sendByte('\r');
//       }

      int scan_iter = 3;
      int i;
      uint16_t sum_IR = 0;
      while(1){
          for(i = 0; i < scan_iter; i++){
              cyBOT_Scan(90, &scan);
              sum_IR += adc_read();
              timer_waitMillis(100);
          }

         uint16_t n = sum_IR / scan_iter;
         sum_IR = 0;


         double dist = 1970000 * pow((double)n, -1.53);

         sprintf(string, "%d\t\t%u\t\t%lf\t\t%lf", i, (unsigned int)n, dist, scan.sound_dist);

         //Send info
         for(j=0; j < strlen(string); j++){
             cyBot_sendByte(string[j]);
         }
         cyBot_sendByte('\n');
         cyBot_sendByte('\r');

         timer_waitMillis(100);

      }


      oi_free(sensor_data); // do this once at end of main()
}
