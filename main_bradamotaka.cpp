#include "digital_in.h"
#include "digital_out.h"
#include "encoder.h"
#include <util/delay.h>
#include <Arduino.h>
#include "timer_msec.h"
#include "analog_out.h"
#include "P_controller.h"

Timer_msec timer;
//Timer_msec encoderTimer;
Digital_out motorIN2(0); //         pin D8
Analog_out motorIN1(1);  // PWM     pin D9
Encoder encoder(3, 4);   // encoder pin D11 D12
P_controller controller(0.1);

int analogPin = A3; 
double ref = 0;
double actual = 0;
double pwmValue = 0;

int main(){  
    init();// Initialize Arduino framework
    Serial.begin(115200);  
    //encoderTimer.init(0.1); // ms
    timer.init(0.1);
    sei();  // enable interrupts
    encoder.init();
    motorIN2.init();
    motorIN1.init(20);  // ms
    motorIN1.set(0);  // duty cycle
    motorIN2.set_lo();

    while(1){
        ref = (analogRead(analogPin)/1023.0)*120;
        actual = encoder.speed();
        pwmValue = controller.update(ref, actual);
        pwmValue = constrain(pwmValue, 0.0, 0.999); // Ensure pwmValue is within [0, 1]
        
        motorIN1.set(pwmValue);
        Serial.print("speed: (");
        Serial.print("Ref: ");
        Serial.print(ref);
        Serial.print(" - Act: ");
        Serial.print(actual);
        Serial.print(") [RPM], ");
        Serial.print(" duty cycle: ");
        Serial.println(pwmValue);
        
        _delay_ms(100);
    }
    return 0;
}

// ISR for Encoder Updates
// ISR(TIMER1_COMPB_vect) {
//     // Update the encoder more frequently
//     encoder.update();
// }

// // Adjusted ISR for PWM Toggle
// ISR(TIMER1_COMPA_vect) {
//     // Toggle PWM at a slower rate
//     motorIN1.pin.set_hi();
// }

// ISR(TIMER2_COMPB_vect) {
//     // Toggle PWM at a slower rate
//     motorIN1.pin.set_lo();
// }

//bráðamótöku code:
// Correct ISR configurations
ISR(TIMER1_COMPA_vect) {
    // Update the encoder
    encoder.update();
}

ISR(TIMER1_COMPB_vect) {
    // Toggle PWM
    motorIN1.pin.toggle(); // Use toggle if suitable, or set_hi/set_lo as needed
}