/*
 * adc.h
 *
 *  Created on: Mar 25, 2022
 *      Author: ibaxter
 */

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>

#ifndef LAB7_CODE_FILES_ADC_H_
#define LAB7_CODE_FILES_ADC_H_

void adc_init (void);
uint16_t adc_read (void);


#endif /* LAB7_CODE_FILES_ADC_H_ */
