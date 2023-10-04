#include "digital_in.h"
#include "digital_out.h"
#include "encoder.h"
#include <util/delay.h>
#include <Arduino.h>
#include "timer_msec.h"
#include "analog_out.h"
#include "P_controller.h"

Timer_msec timer;
Digital_out motorIN2(0); //         pin D8
Analog_out motorIN1(1);  // PWM     pin D9
Encoder encoder(3, 4);   // encoder pin D11 D12
float kp = 1;
P_controller controller(kp);

int analogPin = A3; 
double ref = 0;
double actual = 0;
double pwmValue = 0;
double u = 0;

float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}


int main(){  
    init();// Initialize Arduino framework
    Serial.begin(115200);  
    timer.init(0.1); // ms
    sei();  // enable interrupts

    encoder.init();
    motorIN2.init();
    motorIN1.init(1);  // ms
    motorIN1.set(0);  // duty cycle
    motorIN2.set_lo();
    
    while(1){
        ref = (analogRead(analogPin)/1023.0)*120;
        actual = encoder.speed();
        u = controller.update(ref, actual);
        //pwmValue = constrain(pwmValue, 0.0, 0.999); // Ensure pwmValue is within [0, 1]
        pwmValue = mapFloat(u, -kp*120.0, kp*120.0, 0.0, 0.999);
        //pwmValue = pwmValue/(120);

        motorIN1.set(pwmValue);
        Serial.print("speed: (");
        Serial.print("Ref: ");
        Serial.print(ref);
        Serial.print(" - Act: ");
        Serial.print(actual);
        Serial.print(") [RPM], ");
        Serial.print(" duty cycle: ");
        Serial.print(pwmValue);
        Serial.print(" u: ");
        Serial.println(u);
        
        _delay_ms(100);
    }
    return 0;
}

ISR (INT0_vect){
    encoder.update();
}

ISR(TIMER1_COMPA_vect){
    // encoder.update();
    motorIN1.pin.set_hi();      
}

ISR(TIMER1_COMPB_vect){
    // encoder.update();
    motorIN1.pin.set_lo(); 
}