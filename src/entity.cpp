#include "entity.hpp"

Entity::Entity(int id, const char* texturePath, int x, int y, int width, int height)
{
    this->id = id;
    this->sprite = new Sprite(texturePath, 0, 0, width, height);
    this->position = raylib::Vector2(x,y);
    this->velocity = raylib::Vector2(0,0);
}

Entity::~Entity()
{
    delete this->sprite;
}

void Entity::update()
{
    this->position += this->velocity;
}
void Entity::Draw()
{
    this->sprite->Draw(this->position.x, this->position.y);
}
void Entity::setPosition(float x, float y)
{
    this->position.x = x;
    this->position.y = y;
}
void Entity::setVelocity(float x, float y)
{
    this->velocity.x = x;
    this->velocity.y = y;
}
raylib::Vector2 Entity::getPosition() const
{
    return this->position;
}
raylib::Vector2 Entity::getVelocity() const
{
    return this->velocity;
}