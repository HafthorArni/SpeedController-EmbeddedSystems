#include "digital_in.h"
#include "digital_out.h"
#include "encoder.h"
#include <util/delay.h>
#include <Arduino.h>
#include "timer_msec.h"

Timer_msec timer;
Digital_out led(5);  // Assuming LED is connected to pin 5
Encoder encoder(3, 4);  // Assuming C1 is connected to pin 3 and C2 is connected to pin 4

int main() {
  init();  // Initialize Arduino libraries
  Serial.begin(115200);  // Initialize Serial communication at 9600 baud rate
  timer.init(0.1);
  sei();
  
  encoder.init();
  led.init();

  unsigned long startTime = 0;
  bool startDetected = false;
  unsigned long timeToReachTarget = 0;
  float targetRPM = 0.63 * 120;

  while (1) {
    float currentSpeed = encoder.speed();
    
    // Detect if the motor has started
    if (!startDetected && currentSpeed > 0) {
      startDetected = true;
      startTime = millis();
    }

    if (startDetected && currentSpeed >= targetRPM) {
      timeToReachTarget = millis() - startTime;
      Serial.print("Current Speed: ");
      Serial.print(currentSpeed);
      Serial.print("Time to reach target RPM: ");
      Serial.print(timeToReachTarget);
      Serial.println(" ms");
      _delay_ms(100);
      break; // Stop the loop
    }

    if (startDetected) {
      Serial.print("Current Speed: ");
      Serial.print(currentSpeed);
      Serial.println(" RPM ");
    }

    _delay_ms(100);
  }

  return 0;
}

ISR(TIMER1_COMPA_vect){
    encoder.update();  // Update the encoder position
    led.toggle();
}
