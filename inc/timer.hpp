#ifndef TIMER_HPP
#define TIMER_HPP

#include <raylib-cpp.hpp>

class Timer {
public:
    Timer();
    Timer(float duration, void (*callback)(), bool repeat = false);
    ~Timer();

    void update();
    void Start();
    void Stop();
    float GetElapsedTime() const;
    void setCallback(void (*callback)());
private:
    float duration;
    float currentTime;
    bool isRunning;
    bool repeat;
    void (*callback)();
};

#endif // TIMER_HPP