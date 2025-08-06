#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <raylib-cpp.hpp>

class Sprite
{
    private:
      raylib::Texture2D* texture2D;
      raylib:: Rectangle* rect;
    public:
        Sprite(const char* texturePath, float x, float y, float width, float height);
        Sprite(const char* texturePath, raylib::Rectangle);
        ~Sprite();
        void update();
        void Draw(int x, int y);
};
#endif // SPRITE_HPP