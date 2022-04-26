/*
 * movement.c
 *
 *  Created on: Feb 4, 2022
 *      Author: ibaxter
 */


#include <Final Project Code Files/lcd.h>
#include <Final Project Code Files/open_interface.h>



void move_forward (oi_t *sensor){

    double sum = 0; // distance member in oi_t struct is type double
     oi_setWheels(75, 75); //move forward at full speed

     oi_update(sensor);
     sum += sensor->distance; // use -> notation since pointer

     if(sensor->bumpRight){
         //TODO sensor stuff
         oi_setWheels(0, 0);
     }
     else if(sensor->bumpLeft){
         //TODO sensor stuff
         oi_setWheels(0, 0);
     }

}

void move_backward (oi_t *sensor){

    double sum = 0;
    oi_setWheels(-75, -75);
    oi_update(sensor);
    sum += sensor->distance; // use -> notation since pointer

}


void turn_right(oi_t *sensor){

    double sum = 0;
    oi_setWheels(-69, 69);
    oi_update(sensor);
    sum += sensor->angle;
}

void turn_left(oi_t  *sensor){

    double sum = 0;
    oi_setWheels(69, -69);
    oi_update(sensor);
    sum += sensor->angle;

}





