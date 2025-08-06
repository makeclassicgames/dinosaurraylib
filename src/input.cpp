#include "input.hpp"

Input::Input(){
    for(int i = 0; i < 6; i++){
        this->keyPressed[i] = false;
    }
    this->gamepad = new raylib::Gamepad(0);
}
Input::~Input(){
    // Destructor

}
void Input::Update(){
    // Update input states

   

    this->keyPressed[ACTION_DOWN] = raylib::Keyboard::IsKeyDown(KEY_DOWN) ||
                                    raylib::Keyboard::IsKeyDown(KEY_S) ||
                                    this->gamepad->GetAxisMovement(GAMEPAD_AXIS_LEFT_Y) < -0.5f ||
                                    this->gamepad->IsButtonPressed(GAMEPAD_BUTTON_LEFT_FACE_DOWN);

    this->keyPressed[ACTION_JUMP] = raylib::Keyboard::IsKeyPressed(KEY_SPACE) ||
                                     this->gamepad->IsButtonPressed(GAMEPAD_BUTTON_RIGHT_FACE_DOWN);

    this->keyPressed[ACTION_ENTER] = raylib::Keyboard::IsKeyPressed(KEY_ENTER) ||
                                     raylib::Keyboard::IsKeyPressed(KEY_KP_ENTER) ||
                                     this->gamepad->IsButtonPressed(GAMEPAD_BUTTON_MIDDLE_RIGHT);

}

bool Input::IsActionPerformed(int key){
    return this->keyPressed[key];
}

bool Input::IsAction_Released(int key){
    return !this->keyPressed[key];
}