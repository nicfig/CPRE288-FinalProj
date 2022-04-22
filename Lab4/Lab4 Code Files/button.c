/*
 * button.c
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 *  @edit: Phillip Jones 05/30/2019: Merged Spring 2019 version with Fall 2018
 *  @edit: Diane Rover 02/01/20: Corrected comments about ordering of switches for new LCD board and added busy-wait on PRGPIO
 */



//The buttons are on PORTE 3:0
// GPIO_PORTE_DATA_R -- Name of the memory mapped register for GPIO Port E,
// which is connected to the push buttons
#include <Lab4 Code Files/button.h>

//#define MIN(x,y) (((x) < (y)) ? (x): (y))

//#define BIT0    0x01
//#define BIT1    0x02
//#define BIT2    0x04
//#define BIT3    0x08
//#define BIT4    0x10
//#define BIT5    0x20
//#define BIT6    0x40
//#define BIT7    0x80


/**
 * Initialize PORTE and configure bits 0-3 to be used as inputs for the buttons.
 */
void button_init() {
	static uint8_t initialized = 0;

	//Check if already initialized
	if(initialized){
		return;
	}

	// delete warning after implementing
	//#warning "Unimplemented function: void button_init()"

	// Reading: To initialize and configure GPIO PORTE, visit pg. 656 in the
	// Tiva datasheet.

	// Follow steps in 10.3 for initialization and configuration. Some steps
	// have been outlined below.

	// Ignore all other steps in initialization and configuration that are not
	// listed below. You will learn more about additional steps in a later lab.

	// 1) Turn on PORTE system clock, do not modify other clock enables
	SYSCTL_RCGCGPIO_R |= 0x10; //0b 01 0000

	// You may need to add a delay here of several clock cycles for the clock to start, e.g., execute a simple dummy assignment statement, such as "long delay = SYSCTL_RCGCGPIO_R".

  // Instead, use the PRGPIO register and busy-wait on the peripheral ready bit for PORTE.
	while ((SYSCTL_PRGPIO_R & 0x10) == 0) {
	    long delay = SYSCTL_RCGCGPIO_R;
	};
	// 2) Set the buttons as inputs, do not modify other PORTE wires
	 GPIO_PORTE_DIR_R &= 0xF0;// 0b1111 0000

	// 3) Enable digital functionality for button inputs,
	//    do not modify other PORTE enables
	 GPIO_PORTE_DEN_R |= 0x0F; //0b0000 1111


	initialized = 1;
}



/**
 * Returns the position of the rightmost button being pushed.
 * @return the position of the rightmost button being pushed. 1 is the leftmost button, 4 is the rightmost button.  0 indicates no button being pressed
 */
uint8_t button_getButton() {

	//#warning "Unimplemented function: uint8_t button_getButton(void)"	// delete warning after implementing

	//
	// DELETE ME - How bitmasking works
	// ----------------------------------------
	// In embedded programming, often we only care about one or a few bits in a piece of
	// data.  There are several bitwise operators that we can apply to data in order
	// to "mask" the bits that we don't care about.
	//
	//	| = bitwise OR		& = bitwise AND		^ = bitwise XOR		~ = bitwise NOT
	//		  << x = shift left by x bits		 >> x = shift right by x bits
	//
	// Let's say we want to know if push button 3 (Sw3) is pushed.  Sw3 is bit 2 of GPIO_PORTE_DATA_R.
	// Since push buttons are high (1) initially, and low (0) if pushed, PORTE should
	// look like:
	// GPIO_PORTE_DATA_R => 0b???? ?0?? if Sw3 is pushed
	// GPIO_PORTE_DATA_R => 0b???? ?1?? if Sw3 is not pushed
	//
	// We want to only look at bit 2 and mask the other 7 bits:
	//
	// Bitwise AND:
	// (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0000 if Sw3 is pushed
	// (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0100 if Sw3 is not pushed
	//
	// Bitwise OR:
	// (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1011 if Sw3 is pushed
	// (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1111 if Sw3 is not pushed
	//
	// Other techniques (Shifting and bitwise AND)
	// ((GPIO_PORTE_DATA_R >> 2) & 1) => 0 if Sw3 is pushed
	// ((GPIO_PORTE_DATA_R >> 2) & 1) => 1 if Sw3 is not pushed

	// TODO: Write code below -- Return the rightmost button position pressed

	// INSERT CODE HERE!
    uint8_t data;

    if((GPIO_PORTE_DATA_R & 0b00001000) == 0b00000000){
        data = 0b00001000;
    }
    else if ((GPIO_PORTE_DATA_R & 0b00000100) == 0b00000000){
        data = 0b00000100;
    }
    else if ((GPIO_PORTE_DATA_R & 0b00000010) == 0b00000000){
        data = 0b00000010;
    }
    else if ((GPIO_PORTE_DATA_R & 0b00000001) == 0b00000000){
        data = 0b00000001;
    }


	return data; // EDIT ME
}
