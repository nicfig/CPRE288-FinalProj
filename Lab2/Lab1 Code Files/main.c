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
#include <stdio.h>
#include "string.h"

#include "open_interface.h"
#include "movement.h"

void main()
{
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    double distance_mm = 1000.0;

//    move_forward(sensor_data, 500);
//    turn_right(sensor_data, -90);
//    move_forward(sensor_data, 500);
//    turn_right(sensor_data, -90);
//    move_forward(sensor_data, 500);
//    turn_right(sensor_data, -90);
//    move_forward(sensor_data, 500);

    move_forward(sensor_data, 2000);


    oi_free(sensor_data); // do this once at end of main()
}


