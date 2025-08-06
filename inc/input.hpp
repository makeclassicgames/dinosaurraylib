#ifndef INPUT_HPP
#define INPUT_HPP
#include <raylib-cpp.hpp>

#define ACTION_DOWN 0
#define ACTION_JUMP 1
#define ACTION_ENTER 2

class Input{
    private:
        bool keyPressed[3];
        raylib::Gamepad* gamepad;
    public:
        Input();
        ~Input();
        void Update();
        bool IsActionPerformed(int key);
        bool IsAction_Released(int key);
};
#endif