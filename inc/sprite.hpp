#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <raylib-cpp.hpp>

typedef struct Animation {
    raylib::Texture2D** texture;
    int frameCount;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    int frameTime;
    int currentTime;
} Animation;

class AnimationBuilder{
    private:
        raylib::Texture2D** texture;
        int frameCount;
        int frameWidth;
        int frameHeight;
        int frameTime;
    public:
       AnimationBuilder();
       ~AnimationBuilder();
       void SetTexture(raylib::Texture2D** texture);
       void SetFrameCount(int frameCount);
       void SetFrameWidth(int frameWidth);
       void SetFrameHeight(int frameHeight);
       void SetFrameTime(int frameTime);
       Animation* Build();
};

class Sprite
{
    private:
    std::vector<Animation>* animations;
    int currentAnimation;
    float scaleFactor;
  
    public:
        Sprite(const char* texturePath, float x, float y, float width, float height);
        Sprite(const Animation& initialAnimation);
        ~Sprite();
        void addAnimation(const Animation& animation);
        void setCurrentAnimation(int index);
        void removeAnimation(int index);
        int getAnimationCount() const;
        void setScaleFactor(float scale);
        void update();
        void Draw(int x, int y);
};
#endif // SPRITE_HPP