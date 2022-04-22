/*
 * movement.h
 *
 *  Created on: Feb 4, 2022
 *      Author: ibaxter
 */

#ifndef FINAL_PROJECT_CODE_FILES_MOVEMENT_H_
#define FINAL_PROJECT_CODE_FILES_MOVEMENT_H_

#include <Final Project Code Files/open_interface.h>

double move_backward (oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor, double degrees);
double turn_left(oi_t  *sensor, double degrees);
double move_forward (oi_t *sensor_data, double distance_mm);



#endif /* FINAL_PROJECT_CODE_FILES_MOVEMENT_H_ */
