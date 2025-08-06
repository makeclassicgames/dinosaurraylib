#ifndef GAME_HPP
#define GAME_HPP
#include <raylib-cpp.hpp>
#include <vector>
#include "entity.hpp"
#include "input.hpp"

#define JUMP_FORCE -110.0f // Adjust this value for jump height

enum status
{
    RUNNING,
    PAUSED,
    GAMEOVER
};

class Player: public Entity
{
    private:
        int score;
        int lives;
        bool isJumping = false;
    public:
        Player(int id, const char* texturePath, int x, int y, int width, int height);
        ~Player();
        void setScore(int score);
        int getScore() const;
        void setLives(int lives);
        int getLives() const;
        void update(Input *input);
        float NewFunction();
};

enum EnemyType
{
    ENEMY_TYPE_1,
    ENEMY_TYPE_2,
    ENEMY_TYPE_3
};

class Obstacle: public Entity
{
    private:
        EnemyType type;
    public:
        Obstacle(int id, const char* texturePath, int x, int y, int width, int height, EnemyType type);
        ~Obstacle();

};

class Game
{
    private:
        raylib::Window* window;
        raylib::Color* textColor;
        Entity* miSprite;
        enum status gameStatus;
        Player* player;
        std::vector<Obstacle*> obstacles;
        Input* input;
        raylib::Texture2D* background;
        Obstacle* obstacle1;
    public:
        Game();
        ~Game();
        void Init(int screenWidth, int screenHeight);
        bool isWindowClosed();
        void printText(const char* text, int x, int y, int fontSize);
        void Update();
        void Draw();
        void deInit();
        enum status GetStatus() const;
        void SetStatus(enum status newStatus);

};


#endif