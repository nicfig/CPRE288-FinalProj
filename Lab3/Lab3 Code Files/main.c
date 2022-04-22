/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include <Lab3 Code Files/lcd.h>
#include <Lab3 Code Files/movement.h>
#include <Lab3 Code Files/open_interface.h>
#include <Lab3 Code Files/Timer.h>
#include <stdio.h>
#include "string.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "Timer.h"
#include "open_interface.h"
#include "movement.h"


typedef struct obj{
    int first;
    int second;
    int angle;
} obj;

void main()
{
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    timer_init();
    lcd_init();
    lcd_clear();


    cyBOT_init_Scan(0b0111);
    //cyBOT_SERVO_cal();
    cyBot_uart_init();
    right_calibration_value = 269500;
    left_calibration_value = 1272250;


    cyBOT_Scan_t scan;
    char line1[] ="Degrees\tPING Distance (cm)\tIR Value";



    int j=0;
    for(j = 0; j < strlen(line1); j++){
        cyBot_sendByte(line1[j]);
    }
    cyBot_sendByte('\n');
    cyBot_sendByte('\r');

    char string[100];
    float data[90];
    int i=0;
    int k = 0;


    cyBOT_Scan(0, &scan);
    timer_waitMillis(300);


   for(i = 0; i < 180; i = i + 2){
       cyBOT_Scan(i, &scan);

       sprintf(string, "%d\t%f\t\t%d", i, scan.sound_dist, scan.IR_raw_val);


       data[k] = scan.sound_dist;
       k++;

       //Send info
       for(j=0; j < strlen(string); j++){
           cyBot_sendByte(string[j]);
       }
       cyBot_sendByte('\n');
       cyBot_sendByte('\r');
   }


   i = 0;
   k = 0;
   j = 0;
   int first = 0;
   obj pipe[3];
   while(i < 90){
       if((data[i] <= 50.0) && (data[i] >= 5.0)){
           first = i;
           k += 1;
           while((data[i] <= 50.0) && (data[i] >= 5.0)){
               k += 1;
               i += 1;
           }
           if(k > 4){
               pipe[j].first = first;
               pipe[j].second = i;
               pipe[j].angle = k;
               j++;
           }
           k=0;
       }
       i += 1;
   }

   while(j < 3){
       pipe[j].first = 99999;
       pipe[j].second = 99999;
       pipe[j].angle = 99999;
       j++;
   }

   obj smallest = pipe[0];

   for(i = 1; i < 3; i++){
       if(pipe[i].angle < smallest.angle){
           smallest = pipe[i];
       }
   }



   int obj_angle = (2*smallest.first + 2*smallest.second) / 2;
   cyBOT_Scan(obj_angle, &scan);
   timer_waitMillis(300);
   float dist = scan.sound_dist;


   sprintf(string, "%d\t%d\t%lf", obj_angle, 2*smallest.angle, dist);
   j=0;
   for(j=0; j < strlen(string); j++){
       cyBot_sendByte(string[j]);
   }
   cyBot_sendByte('\n');
   cyBot_sendByte('\r');


   if(obj_angle > 90){
       turn_left(sensor_data, 180-obj_angle);
   }
   else if(obj_angle <= 90){
       turn_right(sensor_data, -(90-obj_angle));
   }

   move_forward(sensor_data, (dist*10) - 150);


// Check 1

//    while(true){
//       char c = cyBot_getByte();
//       lcd_printf("%c", c);
//
//       cyBot_sendByte(c);
//    }


//    while(true){
//        char c = cyBot_getByte();
//        if(c == 'w'){
//            oi_setWheels(500, 500);
//        }
//        else if(c == 's'){
//            oi_setWheels(-500, -500);
//        }
//        else if(c == 'a'){
//            oi_setWheels(500, -500);
//        }
//        else if(c == 'd'){
//            oi_setWheels(-500, 500);
//        }
//    }




//    cyBOT_Scan_t getScan;
//    cyBOT_init_Scan(0b0111);
//
//    //cyBot_SendByte("Degrees PING Distance (cm)  IR Value");
//
//    int i;
//    if(cyBot_getByte() == 'm'){
//        for(i = 60; i < 120; i = i + 2){
//            cyBOT_Scan(i, &getScan);
//            //cyBot_SendByte(getScan->sound_dist);
//            //cyBot_SendByte(' ');
//            //cyBot_SendByte(getScan->IR_raw_val);
//            //cyBot_SendByte('\n');
//        }
//    }






    oi_free(sensor_data); // do this once at end of main()
}


