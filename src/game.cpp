#include "game.hpp"

Animation loadPlayerRunAnimation();
Animation loadPlayerCrouchAnimation();
Animation loadPlayerDeathAnimation();

void obstacleSpawnCallback();

Game::Game()
{
    gameStatus = RUNNING;
}

Game::~Game()
{
    // Destructor
    delete this->player;
    delete this->input;
    delete this->textColor;
    delete this->window;

}

void Game::Init(int screenWidth, int screenHeight)
{
    // Initialization code
    this->textColor = new raylib::Color(BLACK);
    this->window = new raylib::Window(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");
    SetTargetFPS(60);
    this->input = new Input();
    this->background = new raylib::Texture2D("resources/background.png");
    this->player = new Player(1, "resources/idle0.png", 30, 315, 90, 95);
    this->player->addAnimation(loadPlayerRunAnimation());
    this->player->addAnimation(loadPlayerCrouchAnimation());
    this->player->addAnimation(loadPlayerDeathAnimation());
    this->player->setCurrentAnimation(1);
    this->obstacles = new std::vector<Obstacle*>();
   
    this->obstacleTimer = new Timer(180.0f, obstacleSpawnCallback, true); // Spawn obstacles every 2 seconds
}

bool Game::isWindowClosed()
{
    return window->ShouldClose();
}

void Game::Update()
{
    // Update game logic
    this->input->Update();
    this->player->update(this->input);
    this->obstacleTimer->update();
    
    for (size_t i = 0; i < this->obstacles->size(); i++)
    {
        this->obstacles->at(i)->setVelocity(-5.0f, 0.0f);
        this->obstacles->at(i)->update();
    }
    
}

void Game::Draw()
{
    // Draw game elements
    ClearBackground(RAYWHITE);
    this->printText("Welcome to Raylib C++ Starter Kit!", 190, 200, 20);
    // Draw Background
    this->background->Draw((Vector2){0, this->window->GetHeight() - (this->background->GetHeight() * 4.0f)}, 0.0f, 4.0f, WHITE);
    // Draw player
    this->player->Draw();
    // Draw obstacle
    this->obstacle1->Draw();
}

void Game::deInit()
{
    // De-initialization code
    window->Close();
    delete this->background;
}
enum status Game::GetStatus() const
{
    return gameStatus;
}
void Game::SetStatus(enum status newStatus)
{
    gameStatus = newStatus;
}

void Game::printText(const char *text, int x, int y, int fontSize)
{
    this->textColor->DrawText(text, x, y, fontSize);
}

Player::Player(int id, const char *texturePath, int x, int y, int width, int height)
    : Entity(id, texturePath, x, y, width, height)
{
    this->score = 0;
    this->lives = 3;
    this->isCrouching = false;
}

Player::~Player()
{
    delete this->sprite;
}

void Player::update(Input *input)
{
    this->velocity = raylib::Vector2(0, 0);

    if (isJumping && this->position.y < 315)
    {
        this->velocity.y += GRAVITY; // Apply gravity
    }
    else if (this->position.y >= 315 && !this->isCrouching)
    {
        this->isJumping = false; // Reset jumping state when on ground
        this->position.y = 315;  // Ensure player stays on ground
        this->setCurrentAnimation(1);//set run animation
    }

    if (input->IsActionPerformed(ACTION_JUMP) && !this->isJumping)
    {
        this->velocity.y = JUMP_FORCE; // Jump velocity
        this->isJumping = true;
        this->setCurrentAnimation(0);
    }

    if (!isJumping)
    {
        if (input->IsActionPerformed(ACTION_DOWN) && !this->isCrouching)
        {
            this->setCurrentAnimation(2);
            this->position.y += 50; // Move player up when crouching
            this->isCrouching = true; // Set crouching state
        }

        if(isCrouching && input->IsAction_Released(ACTION_DOWN)){
            this->isCrouching = false; // Reset crouching state
            this->position.y -= 50; // Move player down when standing up
            this->setCurrentAnimation(1); // Set to running animation
        }
      
    }

    // TODO Refactor for use entity update
    this->position += this->velocity;
    this->sprite->update();
}

void Player::setLives(int lives)
{
    this->lives = lives;
}
int Player::getLives() const
{
    return this->lives;
}

void Player::setScore(int score)
{
    this->score = score;
}
int Player::getScore() const
{
    return this->score;
}

Obstacle::Obstacle(int id, const char *texturePath, int x, int y, int width, int height, EnemyType type)
    : Entity(id, texturePath, x, y, width, height)
{
    this->type = type;
}

Obstacle::~Obstacle()
{
    delete this->sprite;
}

Animation loadPlayerRunAnimation()
{
    Animation anim;
    anim.texture = new raylib::Texture2D *[2];
    anim.texture[0] = new raylib::Texture2D("resources/run0.png");
    anim.texture[1] = new raylib::Texture2D("resources/run1.png");
    anim.frameCount = 2;
    anim.currentFrame = 0;
    anim.frameWidth = 90;  // Width of the player sprite
    anim.frameHeight = 95; // Height of the player sprite
    anim.frameTime = 10;   // Time per frame in milliseconds
    anim.currentTime = 0;  // Initialize current time
    return anim;
}

Animation loadPlayerCrouchAnimation()
{
    Animation anim;
    anim.texture = new raylib::Texture2D *[2];
    anim.texture[0] = new raylib::Texture2D("resources/crouch0.png");
    anim.texture[1] = new raylib::Texture2D("resources/crouch1.png");
    anim.frameCount = 2;
    anim.currentFrame = 0;
    anim.frameWidth = 118;  // Width of the player sprite
    anim.frameHeight = 62; // Height of the player sprite
    anim.frameTime = 10;   // Time per frame in milliseconds
    anim.currentTime = 0;  // Initialize current time
    return anim;
}

Animation loadPlayerDeathAnimation()
{
    Animation anim;
    anim.texture = new raylib::Texture2D *[2];
    anim.texture[0] = new raylib::Texture2D("resources/death0.png");
    anim.frameCount = 1;
    anim.currentFrame = 0;
    anim.frameWidth = 90;  // Width of the player sprite
    anim.frameHeight = 95; // Height of the player sprite
    anim.frameTime = 10;   // Time per frame in milliseconds
    anim.currentTime = 0;  // Initialize current time
    return anim;
}

void obstacleSpawnCallback(){
    Obstacle* newObstacle = new Obstacle(1, "resources/cactus0.png", 830, 300, 17, 35, ENEMY_TYPE_1);
    newObstacle->setScaleFactor(4.0f); // Set scale factor for obstacle
    obstacles.push_back(newObstacle);
}
