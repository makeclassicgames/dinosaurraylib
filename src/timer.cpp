#include "timer.hpp"

Timer::Timer(float duration, void (*callback)(), bool repeat=false){
    this->duration=duration;
    this->callback = callback;
    this->repeat = repeat;
    this->isRunning = false;
}
    
Timer::~Timer()
{
    this->duration=0;
    this->callback = nullptr;
    this->repeat = false;
    this->isRunning = false;
}

void Timer::Start()
{
    this->currentTime = 0;
    this->isRunning=true;
}

void Timer::Stop()
{
    isRunning = false;
}

void Timer::update(){
    if (isRunning)
    {
        currentTime ++;
        if (currentTime >= duration)
        {
            if (repeat)
            {
                currentTime = 0;
            }
            else
            {
                isRunning = false;
            }
            if (callback)
            {
                callback();
            }
        }
    }
}

void Timer::setCallback(void (*callback)())
{
    this->callback = callback;
}
