#include "digital_in.h"
#include "digital_out.h"
#include "encoder.h"
#include "timer_msec.h"
#include "analog_out.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <Arduino.h>
#include "P_controller.h"

Timer_msec timer;
Analog_out motorIN1(1);  // PWM pin D9
Digital_out motorIN2(0); //     pin D8
Encoder encoder(3, 4);   // Assuming C1 is connected to pin 3 and C2 is connected to pin 4

P_controller controller(0.01);
int main()
{
    double ref = 40;
    Serial.begin(115200);
    timer.init(0.1);
    encoder.init();

    motorIN2.set_lo();
    motorIN1.init(10);  // ms
    motorIN1.set(0);  // duty cycle
    double currentPWM = 0;
    sei();  // enable interrupts

    while(1){
        //Serial.print(encoder.speed());
        //Serial.println(" RPM ");
        double pwmValue = controller.update(ref, encoder.speed());
        currentPWM += pwmValue;
        Serial.println(currentPWM);
        Serial.print(encoder.speed());
        motorIN1.set(currentPWM);
        _delay_ms(100);
    }
}

ISR(TIMER1_COMPA_vect)
{
    motorIN1.pin.set_hi();
    encoder.update();
    //double speed = encoder.speed();

}

ISR(TIMER1_COMPB_vect)
{
    motorIN1.pin.set_lo();
    encoder.update();
}
