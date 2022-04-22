/*
 * servo.c
 *
 *  Created on: Apr 8, 2022
 *      Author: ibaxter
 */


//#include <Lab10 Code Files/lcd.h>
//#include <Lab10 Code Files/servo.h>
//#include <Lab10 Code Files/Timer.h>
//
//
//void servo_init(void){
//    SYSCTL_RCGCGPIO_R |= 0x02;
//    SYSCTL_RCGCTIMER_R |= 0x02;
//    while((SYSCTL_PRGPIO_R & 0x02) != 0x02){};
//    GPIO_PORTB_DIR_R |= 0x20;  // output
//    GPIO_PORTB_AFSEL_R |= 0x20;
//    GPIO_PORTB_PCTL_R |= 0x00007000;
//    GPIO_PORTB_PCTL_R &= 0xFFFF7FFF;
//    GPIO_PORTB_DEN_R |= 0x20;
//
//    TIMER1_CTL_R &= ~0x100;
//    TIMER1_CFG_R = 0x4;
//    TIMER1_TBMR_R = 0x0A;
//    TIMER1_CTL_R |= (0x0C00);
//    TIMER1_TBPR_R = 0xFF;
//    TIMER1_TBILR_R = 0xFFFF;
//    TIMER1_CTL_R |= 0x100;
//}
//void servo_move(uint16_t degrees){
////    uint16_t time = degrees * (0.28 / 60);
////    GPIO_PORTB_DATA_R = time;
//
//    // start value in clock cycles
//    unsigned long pwm_period;
//    // lower 16 bits of the start value
//    TIMER1_TBILR_R = pwm_period & 0x______;
//    // next 8 bits of the start value
//    TIMER1_TBPR_R = pwm_period >> _______;
//
//}

#include <Final Project Code Files/button.h>
#include <Final Project Code Files/servo.h>
#include <Final Project Code Files/Timer.h>
#include <stdint.h>

 int leftVal = 283361;
 int rightVal = 311150;

int new_position = 90;
bool direction_clockwise = false; //false is counterclockwise
bool moved = false;
int x = 0;

void servo_init (void){
    SYSCTL_RCGCGPIO_R |= 0b10;  // enable clock on PORT B of GPIO
    SYSCTL_RCGCTIMER_R |= 0b10; // enable clock for timer 1
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};    // wait for ready on PORT B
    GPIO_PORTB_DIR_R |= 0b100000;    // set PB5 to output
    GPIO_PORTB_DEN_R |= 0b100000;    // enable digital output on PB5

    GPIO_PORTB_AFSEL_R |= 0b100000; // enable alt function on PB5
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF;  // set alt function as T3CCP1 function
    GPIO_PORTB_PCTL_R |= 0x700000;  // ... on PB3 (set bits [15..12] to 7) // correct to PB5

    //--------- TIMER SETUP ----------
    TIMER1_CTL_R &= ~0x100; // disable Timer during startup
    TIMER1_CFG_R = 0x4;     // using 16bit with prescaler
    TIMER1_TBMR_R |= 0b1010;
    TIMER1_TBMR_R &= ~0b0101; // configure Timer B Mode for PWM one-shot
    TIMER1_CTL_R &= ~0x4000; //noniverted

    TIMER1_TBILR_R = 0xE200; //320000 // Loads the starting count value into timer (max value bc
                             // we are counting down)
    TIMER1_TBPR_R = 0x4;      // Loads the starting count value into the remaining 8bit prescaler
                             // (max because we're counting down) (0x04e200)
    TIMER1_TBMATCHR_R = 0xC000; // 20ms low pulse width
    TIMER1_TBPMR_R = 0x4;

    TIMER1_CTL_R |= 0x100; // re-enable Timer B
}


void servo_move(uint16_t degrees){
    // w(d) = (1/180)d+1 (in ms)
    // cycles(w in ms) = 16000000*(w in ms/1000)
//    float highPulse_ms = (1.0/180.0)*degrees+1.0;
//    uint32_t widthCycles = 16000000.0 * (((1.0/180.0)*degrees+1.0)/1000.0);
//    uint32_t lowCycles = 320000 - widthCycles;

    //unsigned int lowCycles = 320000 - ((320000-288000)/180)*degrees;
    unsigned int lowCycles = leftVal - ((leftVal-rightVal)/180)*degrees;

    lcd_printf("%d", lowCycles);

    TIMER1_TBMATCHR_R = (lowCycles & 0xFFFF);
    TIMER1_TBPMR_R = lowCycles>>16;
}

//Everything below this is in progress
void button_move() {
    //timer_waitMillis(100);
    while(x != 1 && x != 2 && x != 3 && x != 4) {
        timer_waitMillis(100);
        x = button_getButton(); //this alone is weak at catching a button press. This needs a rework.
    }
    if(x == 1) {
        moved = true;
        if(direction_clockwise == true) {
            new_position = new_position - 1;
            servo_move(new_position);
        }
        else if(new_position <= 1) {
            new_position = 0;
        }
        else {
            new_position = new_position + 1;
            servo_move(new_position);
        }
    }
    if(x == 2) {
        moved = true;
        if(direction_clockwise == true) {
            new_position = new_position - 5;
            servo_move(new_position);
        }
        else if(new_position <= 5) {
            new_position = 0;
        }
        else {
              new_position = new_position + 5;
              servo_move(new_position);
        }
    }
    if(x == 3 && moved == true) {
        if(direction_clockwise == false) {
            direction_clockwise = true;
            moved = false;
        }
        else {
            direction_clockwise = false;
            moved = false;
        }
    }
    if(x == 4) {
        moved = true;
        if(direction_clockwise == true) {
            new_position = 5;
            servo_move(new_position);
        }
        else {
            new_position = 175;
            servo_move(new_position);
        }
    }
    x = 0;
    return;
}

