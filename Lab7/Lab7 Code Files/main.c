/**
 * lab6_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "string.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "Timer.h"
#include "open_interface.h"
#include "movement.h"


#include "uart.h"
#include "adc.h"

// Uncomment or add any include directives that are needed
// #include "button.h"


//#warning "Possible unimplemented functions"
#define REPLACEME 0

typedef struct obj{
    int first;
    int second;
    int angle;
} obj;

typedef struct obj_PING{
    int angle;
    float distance;
    float width;
} obj_PING;

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

       cyBot_sendByte('\n');
       cyBot_sendByte('\r');

       char line1[] = "Degrees\t\tIR\t\tis_Object";

       int j = 0;

       for(j = 0; j < strlen(line1); j++){
           cyBot_sendByte(line1[j]);
       }
       cyBot_sendByte('\n');
       cyBot_sendByte('\r');

       char string[100];
       float data_IR[180];
       int i = 0;
       int k = 0;
       int first_angle = 0;

       bool is_Object = false;
       obj objects[10];

       cyBOT_Scan(0, &scan);
       timer_waitMillis(300);

       int scan_amt = 3;

      for(i = 0; i < 180; i++){
          int sum_IR = 0;
          int j;
          for(j = 0; j < scan_amt; j++){
              cyBOT_Scan(i, &scan);
              sum_IR += adc_read();
          }

          float avg_IR = sum_IR / scan_amt;

          data_IR[i] = avg_IR;



          if(avg_IR > 750){
              if(!is_Object){
                  objects[k].first = i;
                  is_Object = true;
                  first_angle = i;
              }
          }
          else{
              if(is_Object){
                  objects[k].second = i-1;
                  objects[k].angle = i - first_angle - 1;
                  first_angle = 0;
                  k++;
              }
              is_Object = false;
          }

          sprintf(string, "%d\t\t%f\t\t%i", i, avg_IR, is_Object);

          for(j=0; j < strlen(string); j++){
              cyBot_sendByte(string[j]);
          }

          cyBot_sendByte('\n');
          cyBot_sendByte('\r');

      }

      while(k < 10){
          objects[k].first = -1;
          objects[k].second = -1;
          k++;
      }


      obj_PING data_PING[10];

      char line2[] = "Angle\t\tDistance (cm)\t\tWidth (cm)";

      cyBot_sendByte('\n');
      cyBot_sendByte('\r');

      for(j = 0; j < strlen(line2); j++){
          cyBot_sendByte(line2[j]);
      }

      cyBot_sendByte('\n');
      cyBot_sendByte('\r');

      for(i = 0; i < 10; i++){
          if((objects[i].first > -1) && (objects[i].second > -1)){
              int midpoint = (objects[i].first + objects[i].second) / 2;
              cyBOT_Scan(midpoint, &scan);
              data_PING[i].angle = midpoint;
              data_PING[i].distance = scan.sound_dist;
              data_PING[i].width = 2.0 * M_PI * scan.sound_dist * ((double)objects[i].angle/360);

              sprintf(string, "%d\t\t%f\t\t%f\t\t%d", midpoint, scan.sound_dist, data_PING[i].width, objects[i].angle);

              //Send info
              for(j=0; j < strlen(string); j++){
                  cyBot_sendByte(string[j]);
              }

              cyBot_sendByte('\n');
              cyBot_sendByte('\r');
          }
          else{
              break;
          }


      }

      int end_data_1 = i;

      //move_forward(sensor_data, 115);
      turn_right(sensor_data, -180); // turns 180 degrees

        for(j = 0; j < strlen(line1); j++){
            cyBot_sendByte(line1[j]);
        }
        cyBot_sendByte('\n');
        cyBot_sendByte('\r');

        obj objects2[10];

        cyBOT_Scan(0, &scan);
        timer_waitMillis(300);

        k=0;
        for(i = 0; i < 180; i++){
                  int sum_IR = 0;
                  int j;
                  for(j = 0; j < scan_amt; j++){
                      cyBOT_Scan(i, &scan);
                      sum_IR += scan.IR_raw_val;
                  }

                  float avg_IR = sum_IR / scan_amt;

                  data_IR[i] = avg_IR;



                  if(avg_IR > 750){
                      if(!is_Object){
                          objects2[k].first = i;
                          is_Object = true;
                          first_angle = i;
                      }
                  }
                  else{
                      if(is_Object){
                          objects2[k].second = i-1;
                          objects2[k].angle = i - first_angle - 1;
                          first_angle = 0;
                          k++;
                      }
                      is_Object = false;
                  }

                  sprintf(string, "%d\t\t%f\t\t%i", i, avg_IR, is_Object);

                  for (j=0; j < strlen(string); j++){
                      cyBot_sendByte(string[j]);
                  }

                  cyBot_sendByte('\n');
                  cyBot_sendByte('\r');

          }

          while(k < 10){
              objects2[k].first = -1;
              objects2[k].second = -1;
              k++;
          }

          cyBot_sendByte('\n');
          cyBot_sendByte('\r');

          for(j = 0; j < strlen(line2); j++){
              cyBot_sendByte(line2[j]);
          }

          cyBot_sendByte('\n');
          cyBot_sendByte('\r');

          obj_PING data_PING2[10];

          for(i = 0; i < 10; i++){
              if((objects2[i].first > -1) && (objects2[i].second > -1)){
                  int midpoint = (objects2[i].first + objects2[i].second) / 2;
                  cyBOT_Scan(midpoint, &scan);
                  data_PING2[i].angle = midpoint;
                  data_PING2[i].distance = scan.sound_dist;
                  data_PING2[i].width = 2.0 * M_PI * scan.sound_dist * ((double)objects2[i].angle/360);

                  sprintf(string, "%d\t\t%f\t\t%f\t\t%d", midpoint, scan.sound_dist, data_PING2[i].width, objects2[i].angle);

                  //Send info
                  for(j=0; j < strlen(string); j++){
                      cyBot_sendByte(string[j]);
                  }

                  cyBot_sendByte('\n');
                  cyBot_sendByte('\r');
              }
              else{
                  break;
              }
           }

          int end_data_2 = i;

          obj_PING smallest;
          smallest.angle = 9999;
          smallest.distance = 9999;
          smallest.width = 9999;

          bool inSet1 = true;

          i=0;
          for(i = 0; i < end_data_1; i++){
              if((data_PING[i].width < smallest.width) && (data_PING[i].width > 3.75)){
                  smallest = data_PING[i];
              }
          }

          for(i = 0; i < end_data_2; i++){
              if((data_PING2[i].width < smallest.width) && (data_PING2[i].width > 3.75)){
                  smallest = data_PING2[i];
                  inSet1 = false;
              }
          }


          for(j = 0; j < strlen(line2); j++){
              cyBot_sendByte(line2[j]);
          }


          sprintf(string, "%d\t\t%f\t\t%f", smallest.angle, smallest.distance, smallest.width);
          cyBot_sendByte('\n');
          cyBot_sendByte('\r');

          //Send info
          for(j=0; j < strlen(string); j++){
              cyBot_sendByte(string[j]);
          }

          cyBot_sendByte('\n');
          cyBot_sendByte('\r');

          if(inSet1){
              turn_right(sensor_data, -180);
              //move_backward(sensor_data, -100);
          }
          else{
             // move_forward(sensor_data, 100);
          }

          timer_waitMillis(300);

          if(smallest.angle > 90){
              turn_left(sensor_data, smallest.angle - 90);
          }
          else if(smallest.angle <= 90){
              turn_right(sensor_data, -(90-smallest.angle));
          }

          move_forward(sensor_data, (smallest.distance*10) - 150);


          oi_free(sensor_data); // do this once at end of main()
}
