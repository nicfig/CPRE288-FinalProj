/*
 * movement.h
 *
 *  Created on: Feb 4, 2022
 *      Author: ibaxter
 */

#ifndef LAB7_CODE_FILES_MOVEMENT_H_
#define LAB7_CODE_FILES_MOVEMENT_H_

#include "open_interface.h"

double move_backward (oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor, double degrees);
double turn_left(oi_t  *sensor, double degrees);
double move_forward (oi_t *sensor_data, double distance_mm);



#endif /* LAB7_CODE_FILES_MOVEMENT_H_ */
