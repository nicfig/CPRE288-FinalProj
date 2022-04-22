/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

//#include <Lab10 Code Files/lcd.h>
//#include <Lab10 Code Files/ping.h>
//#include <Lab10 Code Files/Timer.h>
//
//volatile unsigned long START_TIME = 0;
//volatile unsigned long END_TIME = 0;
//volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse
//
//int count = 0;
//
//bool intFlag = true;
//
//void ping_init (void){
//
//  // YOUR CODE HERE
//    SYSCTL_RCGCGPIO_R |= 0x02;
//    SYSCTL_RCGCTIMER_R |= 0x08;
//    while((SYSCTL_PRGPIO_R & 0x02) != 0x02){};
//    GPIO_PORTB_DIR_R &= ~0x08; //input
////    GPIO_PORTB_AFSEL_R &= ~0x08;
//    GPIO_PORTB_AFSEL_R |= 0x08;
//    GPIO_PORTB_PCTL_R |= 0x00007000;
//    GPIO_PORTB_PCTL_R &= 0xFFFF7FFF;
//    GPIO_PORTB_DEN_R |= 0x08;
//
//
//    // Configure and enable the timer
//    TIMER3_CTL_R &= ~0x100;
//    TIMER3_CFG_R = 0x4;
//    TIMER3_TBMR_R = 0x07;
//    TIMER3_CTL_R |= (0x0C00);
//    TIMER3_TBPR_R = 0xFF;
//    TIMER3_TBILR_R = 0xFFFF;
//    TIMER3_ICR_R |= 0x400;
//    TIMER3_IMR_R |= 0x400;
//    NVIC_EN1_R = 0x00000010;
//    NVIC_PRI9_R = 0x8; //priority to T3
//    IntRegister(INT_TIMER3B, TIMER3B_Handler);
//
//    IntMasterEnable();
//    TIMER3_CTL_R |= 0x100;
//}
//
//void ping_trigger (void){
//    STATE = LOW;
//    // Disable timer and disable timer interrupt
//    TIMER3_CTL_R &= ~0X100;
//    TIMER3_IMR_R = ~0x400;
//    // Disable alternate function (disconnect timer from port pin)
//    GPIO_PORTB_AFSEL_R &= ~0x08;
//
//    GPIO_PORTB_DIR_R |= 0x08;  // output
//    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
//
//    GPIO_PORTB_DATA_R &= ~0x08;
//    timer_waitMicros(5);
//    GPIO_PORTB_DATA_R |= 0x08;
//    timer_waitMicros(5);
//    GPIO_PORTB_DATA_R &= ~0x08;
//
//
//    GPIO_PORTB_DIR_R &= ~0x08; //input
//
//    // Clear an interrupt that may have been erroneously triggered
//    TIMER3_ICR_R |= 0x400;
//    // Re-enable alternate function, timer interrupt, and timer
//    GPIO_PORTB_AFSEL_R |= 0x08;
//    TIMER3_IMR_R |= 0x400;
//    TIMER3_CTL_R |= 0X100;
//}
//
//void TIMER3B_Handler(void){
//
//  // YOUR CODE HERE
//   if(TIMER3_MIS_R & 0x400){
//       TIMER3_ICR_R |= 0x400;
//       if (STATE == LOW) {
//           START_TIME = TIMER3_TBR_R;
//           STATE = HIGH;
//       }
//       else if (STATE == HIGH) {
//           END_TIME = TIMER3_TBR_R;
//           STATE = DONE;
//       }
//
//   }
//}
//
//float ping_getDistance (void){
//
//    // YOUR CODE HERE
//    /*
//     * call ping trigger
//     *  wait here until both edges are recorded
//     *  turn clocks to time
//     *  turn time to dist
//     *  return dist
//     */
//    ping_trigger();
//    while(STATE != DONE){};
//    float time_diff = 0;
//    int overflow_count = 0;
//    int overflow = (END_TIME > START_TIME);
//    if(START_TIME > END_TIME){
//        time_diff = START_TIME - END_TIME;
//        lcd_printf("%d  %d", time_diff, count);
//    }
//    else{
//        time_diff = ((unsigned long)overflow<<24) + START_TIME - END_TIME;
//        count++;
//        lcd_printf("%d  %d", time_diff, count);
//    }
//
//    time_diff /= 16000000;
//
//    return (time_diff / 2) * 343 * 100;
//
//}

#include "ping.h"
#include "Timer.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile unsigned long TIME_DIFF = 0;
volatile int update_flag = 0;
int overflow = 0;
enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){
    SYSCTL_RCGCGPIO_R |= 0b10;  // enable clock on PORT B of GPIO
    SYSCTL_RCGCTIMER_R |= 0b1000; // enable clock for timer 3
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};    // wait for ready on PORT B
    GPIO_PORTB_DIR_R &= ~0b1000;    // set PB3 to input
    GPIO_PORTB_DEN_R |= 0b1000;    // enable dig input on PB3

    GPIO_PORTB_AFSEL_R |= 0b1000; // enable alt function on PB3
    GPIO_PORTB_PCTL_R &= 0xFFFF7FFF;  // set alt function as T3CCP1 function
    GPIO_PORTB_PCTL_R |= 0x7000;  // ... on PB3 (set bits [15..12] to 7)

    TIMER3_CTL_R &= 0b011111111; // clear bit 8 to disable timer as we set it up
    TIMER3_CTL_R |= 0b110000000000; // configure the timer
    TIMER3_CFG_R = 0x4;
    TIMER3_TBMR_R &= 0b01111; // clear bit 4 to count down
    TIMER3_TBMR_R |= 0b111; // set bit 2 for edge-time, set bits 1, 0 for capture mode
    TIMER3_TBILR_R = 0xFFFF; // Loads the starting count value into timer (max value bc
                             // we are counting down)
    TIMER3_TBPR_R = 0xFF;    // Loads the starting count value into the remaining 8bit prescaler
                             // (max because we're counting down)
    TIMER3_ICR_R |= 0x400; // clear capture interrupt flag
    TIMER3_IMR_R |= 0x400; // set bit 10 in IMR to enable TB capture interrupt

    NVIC_EN1_R |= 0b10000; // enable interrupt for Timer 3B
    NVIC_PRI9_R &= ~0b11000000; // set priority for bits 7-5 in NVIC Priority (Timer 3B)
    NVIC_PRI9_R |=  0b00100000;
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();

    TIMER3_CTL_R |= 0x100; // re-enable Timer B
}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0x400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0b1000;
    GPIO_PORTB_DIR_R |= 0b1000;    // set PB3 to output

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DATA_R &= 0b0111; // set bit 3 low
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= 0b1000; // set bit 3 high
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= 0b0111; // set bit 3 low

    GPIO_PORTB_DIR_R &= ~0b1000;    // set PB3 to input
    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0b1000; // enable alt function on PB3
    TIMER3_IMR_R |= 0x400; // set bit 10 in IMR to enable TB capture interrupt
    TIMER3_CTL_R |= 0x100; // re-enable Timer B
    STATE = LOW;
}

void TIMER3B_Handler(void){
    // MIS register check to prevent mistaken noise triggering (cross-coupling capacitance)
    if(STATE == LOW){
        // rising edge
        START_TIME = TIMER3_TBV_R;
        STATE = HIGH;
    } else if(STATE == HIGH){
        // output is low, detected falling edge?
        END_TIME = TIMER3_TBV_R;
        update_flag = 1;
        STATE = DONE;
    }
    TIMER3_ICR_R |= 0x400; // consider this output handled, clear flag
}

float ping_getDistance (void){
    // activate ping, wait for our sensor via update flag
    ping_trigger();
    while(STATE != DONE) {};
    STATE = LOW;

    // handle overflow
    overflow = (END_TIME > START_TIME);


    START_TIME += (unsigned long)overflow<<24;
    TIME_DIFF = START_TIME - END_TIME;


    return TIME_DIFF / 16000000.0 / 2.0 * 34300;

}

int getOverflow(){
    return overflow;
}


unsigned long ping_getPulse(void){
    // activate ping, wait for our sensor via update flag
    ping_trigger();
    while(STATE != DONE) {};
    STATE = LOW;

    // handle overflow
    overflow = (END_TIME > START_TIME);


    START_TIME += (unsigned long)overflow<<24;
    TIME_DIFF = START_TIME - END_TIME;


    return TIME_DIFF;
}

