#ifndef TIMER_HPP
#define TIMER_HPP

#include <raylib-cpp.hpp>
#include <functional>

class Timer {
public:
    Timer();
    Timer(float duration, std::function<void()> callback, bool repeat = false);
    ~Timer();

    void update();
    void Start();
    void Stop();
    float GetElapsedTime() const;
    void setCallback(std::function<void()> callback);
private:
    float duration;
    float currentTime;
    bool isRunning;
    bool repeat;
    std::function<void()> callback;
};

#endif // TIMER_HPP