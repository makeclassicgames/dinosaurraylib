#include "sprite.hpp"


Sprite::Sprite(const char* texturePath, float x, float y, float width, float height) {
    this->animations = new std::vector<Animation>();
    AnimationBuilder* builder = new AnimationBuilder();
    builder->SetTexture(new raylib::Texture2D*[1]{ new raylib::Texture2D(texturePath) });
    builder->SetFrameCount(1);
    builder->SetFrameWidth(static_cast<int>(width));
    builder->SetFrameHeight(static_cast<int>(height));
    builder->SetFrameTime(10);
    Animation initialAnimation = *builder->Build();
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

Animation * Sprite::getCurrentAnimation() const {
    return &this->animations->at(this->currentAnimation);
}

//Animation Builder

AnimationBuilder::AnimationBuilder() : texture(nullptr), frameCount(0), frameWidth(0), frameHeight(0), frameTime(0) {
    // Nothing.
}

AnimationBuilder::~AnimationBuilder() {
    if (texture) {
        for (int i = 0; i < frameCount; ++i) {
            delete texture[i];
        }
        delete[] texture;
    }
}

void AnimationBuilder::SetTexture(raylib::Texture2D** texture) {
    this->texture = texture;
}

void AnimationBuilder::SetFrameCount(int frameCount) {
    this->frameCount = frameCount;
}

void AnimationBuilder::SetFrameWidth(int frameWidth) {
    this->frameWidth = frameWidth;
}

void AnimationBuilder::SetFrameHeight(int frameHeight) {
    this->frameHeight = frameHeight;
}

void AnimationBuilder::SetFrameTime(int frameTime) {
    this->frameTime = frameTime;
}

Animation* AnimationBuilder::Build() {
    Animation* anim = new Animation();
    anim->texture = this->texture;
    anim->frameCount = this->frameCount;
    anim->frameWidth = this->frameWidth;
    anim->frameHeight = this->frameHeight;
    anim->frameTime = this->frameTime;
    anim->currentFrame = 0;
    anim->currentTime = 0;
    return anim;
}