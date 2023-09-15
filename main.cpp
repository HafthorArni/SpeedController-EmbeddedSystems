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
  Serial.begin(9600);  // Initialize Serial communication at 9600 baud rate
  timer.init(0.1);
  sei();
  
  encoder.init();
  led.init();

  while (1) {
  Serial.print("Number rev: ");
  Serial.print(encoder.position()/1400);
  Serial.print(" Speed: ");
  Serial.print(encoder.speed());
  Serial.println(" RPM ");

  _delay_ms(100);
}
  
  return 0;
}

ISR(TIMER1_COMPA_vect){
    encoder.update();  // Update the encoder position
    led.toggle();
}
