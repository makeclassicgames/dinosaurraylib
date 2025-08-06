#include "sprite.hpp"

Sprite::Sprite(const char* texturePath, float x, float y, float width, float height){
    this->texture2D = new raylib::Texture2D(texturePath);
    this->rect = new raylib::Rectangle(x,y,width,height);
}

Sprite::Sprite(const char* texturePath, raylib::Rectangle rectangle){
    this->texture2D = new raylib::Texture2D(texturePath);
    this->rect = new raylib::Rectangle(rectangle);
}

Sprite::~Sprite(){
    this->texture2D->Unload();
    delete this->texture2D;
    delete this->rect;
}

void Sprite::update(){
    //TODO

}

void Sprite::Draw(int x, int y ){
    this->texture2D->Draw((Vector2){x,y});
}