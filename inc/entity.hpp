#ifndef ENTITY_HPP
#include <raylib-cpp.hpp>

#include "sprite.hpp"

#define GRAVITY 1.0f

class Entity{
    protected:
    int id;
    Sprite* sprite;
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    public:
        Entity(int id, const char* texturePath, int x, int y, int width, int height);
        ~Entity();
        void update();
        void Draw();
        void setPosition(float x, float y);
        void setVelocity(float x, float y);
        raylib::Vector2 getPosition() const;
        raylib::Vector2 getVelocity() const;
};
#endif