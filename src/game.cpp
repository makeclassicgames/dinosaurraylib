#include "game.hpp"

Game::Game(){
    gameStatus = RUNNING;
}

Game::~Game(){
    // Destructor
    delete this->textColor;
    delete this->window;
    delete this->miSprite;
    delete this->player;
    delete this->input;
}

void Game::Init(int screenWidth, int screenHeight){
    // Initialization code
    this->textColor = new raylib::Color(BLACK);
    this->window = new raylib::Window(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");
    SetTargetFPS(60);
    this->input = new Input();
    this->background = new raylib::Texture2D("resources/background.png");
    this->player = new Player(1, "resources/idle0.png", 30, 315, 90, 95);
    this->obstacle1 = new Obstacle(1, "resources/cactus0.png", 830, 330, 17, 35, ENEMY_TYPE_1);
}

bool Game::isWindowClosed(){
    return window->ShouldClose();
}

void Game::Update(){
    // Update game logic
    this->input->Update();
    this->player->update(this->input);
    this->obstacle1->setVelocity(-5.0f,0.0f);
    this->obstacle1->update();
}

void Game::Draw(){
    // Draw game elements
    ClearBackground(RAYWHITE);
    this->printText("Welcome to Raylib C++ Starter Kit!", 190, 200, 20);
    //Draw Background
    this->background->Draw((Vector2){0, this->window->GetHeight()-(this->background->GetHeight()*4.0f)},0.0f, 4.0f,WHITE);
    // Draw player
    this->player->Draw();
    // Draw obstacle
    this->obstacle1->Draw();
}

void Game::deInit(){
    // De-initialization code
    window->Close();
    delete this->background;
}
enum status Game::GetStatus() const {
    return gameStatus;
}
void Game::SetStatus(enum status newStatus) {
    gameStatus = newStatus;
}

void Game::printText(const char* text, int x, int y, int fontSize){
    this->textColor->DrawText(text, x, y, fontSize);
}


Player::Player(int id, const char* texturePath, int x, int y, int width, int height)
: Entity(id, texturePath, x, y, width, height){
    this->score = 0;
    this->lives = 3;
}

Player::~Player(){
    delete this->sprite;
}

void Player::update(Input* input){
    this->velocity = raylib::Vector2(0,0);
    
    if(isJumping && this->position.y < 315){
        this->velocity.y += GRAVITY; // Apply gravity
    } else if (this->position.y >= 315) {
        this->isJumping = false; // Reset jumping state when on ground
        this->position.y = 315; // Ensure player stays on ground
    }

    if(input->IsActionPerformed(ACTION_JUMP) && !this->isJumping){
        this->velocity.y = JUMP_FORCE; // Jump velocity
        this->isJumping = true;
    }

    if(input->IsActionPerformed(ACTION_DOWN) && !this->isJumping){
        this->sprite= new Sprite("resources/crouch0.png",this->position.x, this->position.y, 90,95);
    }else{
        this->sprite= new Sprite("resources/idle0.png", this->position.x, this->position.y, 90, 95);
    }
    
    this->position += this->velocity;
}

void Player::setLives(int lives){
    this->lives = lives;
}
int Player::getLives() const{
    return this->lives;
}

void Player::setScore(int score){
    this->score = score;
}
int Player::getScore() const{
    return this->score;
}


Obstacle::Obstacle(int id, const char* texturePath, int x, int y, int width, int height, EnemyType type)
:Entity(id, texturePath, x, y, width, height){
    this->type = type;
}

Obstacle::~Obstacle(){
    delete this->sprite;
}


