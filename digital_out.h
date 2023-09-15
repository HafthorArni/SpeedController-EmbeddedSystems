#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

#include <stdint.h> 
#include <avr/io.h>
#include <util/delay.h>

class Digital_out {
    public:
    Digital_out(int pin);
    void init();
    void set_hi();
    void set_lo();
    void toggle();
    private: 
    uint8_t pinMask;
};

Digital_out::Digital_out(int pin)
{
    pinMask = (1 << pin);
}
void Digital_out::init(){
    DDRB |= pinMask;
}
void Digital_out::set_hi(){
    PORTB |= pinMask;
}
void Digital_out::set_lo(){
    PORTB &= ~pinMask;
}
void Digital_out::toggle()
{
    PINB = pinMask;  
}

#endif // DIGITAL_OUT_H


