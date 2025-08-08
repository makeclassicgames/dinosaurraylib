#include "sprite.hpp"


Sprite::Sprite(const char* texturePath, float x, float y, float width, float height) {
    this->animations = new std::vector<Animation>();
    Animation initialAnimation;
    initialAnimation.texture = new raylib::Texture2D*[1];
    initialAnimation.texture[0] = new raylib::Texture2D(texturePath);
    initialAnimation.frameCount = 1;
    initialAnimation.currentFrame = 0;
    initialAnimation.frameWidth = static_cast<int>(width);
    initialAnimation.frameHeight = static_cast<int>(height);
    initialAnimation.frameTime = 10; // Default frame time
    initialAnimation.currentTime = 0;

    this->animations->push_back(initialAnimation);
    this->currentAnimation = 0;
    this->scaleFactor = 1.0f; // Default scale factor
}

Sprite::Sprite(const Animation& initialAnimation) {
    this->animations = new std::vector<Animation>();
    this->animations->push_back(initialAnimation);
    this->currentAnimation = 0;
   
}

Sprite::~Sprite(){
    delete this->animations;
 
}

void Sprite::addAnimation(const Animation& animation) {
    this->animations->push_back(animation);
}

void Sprite::setCurrentAnimation(int index) {
    if (index >= 0 && index < this->animations->size()) {
        this->currentAnimation = index;
    }
}

void Sprite::removeAnimation(int index) {
    if (index >= 0 && index < this->animations->size()) {
        this->animations->erase(this->animations->begin() + index);
    }
}

int Sprite::getAnimationCount() const {
    return this->animations->size();
}

void Sprite::update(){

    Animation* currentAnim = &this->animations->at(this->currentAnimation);
    currentAnim->currentTime++;
    if (currentAnim->currentTime >= currentAnim->frameTime) {
        currentAnim->currentFrame++;
        currentAnim->currentTime = 0;
        if (currentAnim->currentFrame >= currentAnim->frameCount) {
            currentAnim->currentFrame = 0;
        }
    }
}

void Sprite::Draw(int x, int y ){
    Animation* currentAnim = &this->animations->at(this->currentAnimation);
    raylib::Texture2D* currentTexture = currentAnim->texture[currentAnim->currentFrame];
    currentTexture->Draw((Vector2){x,y},0.0f,this->scaleFactor);
}

void Sprite::setScaleFactor(float scale) {
    this->scaleFactor = scale;
}