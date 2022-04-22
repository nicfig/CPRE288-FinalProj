/*
 * movement.c
 *
 *  Created on: Feb 4, 2022
 *      Author: ibaxter
 */


#include "lcd.h"
#include "open_interface.h"

void move_backward (oi_t *sensor_data, double distance_mm){

    lcd_init();

    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-100, -100); //move forward at full speed
    while (sum > distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance; // use -> notation since pointer
    }
    oi_setWheels(0, 0); //stop

}


void turn_right(oi_t *sensor, double degrees){

    double sum = 0;
    oi_setWheels(-69, 69);
    while(sum > degrees){
        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0, 0);
}

void turn_left(oi_t  *sensor, double degrees){

    double sum = 0;
    oi_setWheels(69, -69);
    while(sum < degrees){
        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0, 0);

}

void move_forward (oi_t *sensor_data, double distance_mm){

    lcd_init();

    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(75, 75); //move forward at full speed
    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance; // use -> notation since pointer
        lcd_printf("Distance: %lf", sum);

        if(sensor_data->bumpRight){
            oi_setWheels(0, 0);
            move_backward(sensor_data, -7);
            //distance_mm += 75;
            turn_left(sensor_data, 40);
            move_forward(sensor_data, 165);
            turn_right(sensor_data, -65);
            move_forward(sensor_data, 7);
            move_forward(sensor_data, distance_mm - sum);
        }
        else if(sensor_data->bumpLeft){
            oi_setWheels(0, 0);
            move_backward(sensor_data, -7);
            //distance_mm += 75;
            turn_right(sensor_data, -40);
            move_forward(sensor_data, 165);
            turn_left(sensor_data, 65);
            move_forward(sensor_data, 7);
            move_forward(sensor_data, distance_mm - sum);
        }
    }
    oi_setWheels(0, 0); //stop

}

void move_forward2 (oi_t *sensor_data, double distance_mm){

    lcd_init();

    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(75, 75); //move forward at full speed
    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance; // use -> notation since pointer
        lcd_printf("Distance: %lf", sum);

    }
    oi_setWheels(0, 0); //stop

}


