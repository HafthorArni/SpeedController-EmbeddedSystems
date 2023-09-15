#pragma once

class Timer_msec
{
public:
    Timer_msec();
    void init(float period_ms);
    void init(float period_ms, float duty_cycle);
    void set_duty_cycle(float duty_cycle);
};

