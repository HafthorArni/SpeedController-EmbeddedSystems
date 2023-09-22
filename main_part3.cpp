#include "digital_out.h"
#include "timer_msec.h"
#include "analog_out.h"
#include <avr/interrupt.h>
#include <util/delay.h>

Analog_out motorIN1(3);
Analog_out motorIN2(2);
int main()
{
    motorIN1.init(10);  // ms
    motorIN1.set(0.5);  // duty cycle
    motorIN2.init(10);  // ms 
    motorIN2.set(0);  // duty cycle
    sei();  // enable interrupts
}


ISR(TIMER1_COMPA_vect)
{
    // action to be taken at the start of the on-cycle
    motorIN1.pin.set_hi();
    //motorIN2.pin.set_hi();
}

ISR(TIMER1_COMPB_vect)
{
    // action to be taken at the start of the off-cycle
    motorIN1.pin.set_lo();
    //motorIN2.pin.set_lo();
}
