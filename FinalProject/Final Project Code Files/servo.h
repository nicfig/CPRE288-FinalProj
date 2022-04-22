/*
 * servo.h
 *
 *  Created on: Apr 8, 2022
 *      Author: ibaxter
 */
#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"


#ifndef FINAL_PROJECT_CODE_FILES_SERVO_H_
#define FINAL_PROJECT_CODE_FILES_SERVO_H_

void servo_init(void);
void servo_move(uint16_t degrees);
void button_move(void);

#endif /* FINAL_PROJECT_CODE_FILES_SERVO_H_ */
