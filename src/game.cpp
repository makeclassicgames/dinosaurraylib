#include "game.hpp"

#define DEBUG 0

Animation loadPlayerRunAnimation();
Animation loadPlayerCrouchAnimation();
Animation loadPlayerDeathAnimation();
Animation loadPetroAnimation();

void obstacleSpawnCallback();

Game::Game()
{
    gameStatus = START;
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
    this->background[0] = new raylib::Texture2D("resources/background.png");
    this->background[1] = new raylib::Texture2D("resources/background.png");
    this->player = new Player(1, "resources/idle0.png", 30, 315, 90, 95);
    this->backgroundOffset = 0;
    this->player->addAnimation(loadPlayerRunAnimation());
    this->player->addAnimation(loadPlayerCrouchAnimation());
    this->player->addAnimation(loadPlayerDeathAnimation());
    this->player->setCurrentAnimation(1);
    this->obstacles = new std::vector<Obstacle *>();

    this->obstacleTimer = new Timer(180.0f, [this]()
                                    { this->spawnObstacle(); }, true); // Spawn obstacles every 3 seconds
    this->obstacleTimer->Start();
}

bool Game::isWindowClosed()
{
    return window->ShouldClose();
}

void Game::Update()
{
    // Update game logic
    this->input->Update();
    if (this->gameStatus == START && this->input->IsActionPerformed(ACTION_JUMP))
    {
        this->SetStatus(RUNNING);
    }
    else
    {
        if (this->gameStatus == RUNNING)
        {
            this->player->update(this->input);
            this->obstacleTimer->update();
        }
    }
    CollisionRect playerCollider = this->player->getCollider();
    for (size_t i = 0; i < this->obstacles->size(); i++)
    {
        Obstacle *obstacle = this->obstacles->at(i);
        if (obstacle->getPosition().x < -100)
        {
            // Remove the obstacle from the vector
            this->obstacles->erase(this->obstacles->begin() + i);
            i--; // Decrement i to account for the removed obstacle
            if (obstacle->getType() == ENEMY_TYPE_PETRO)
            {
                this->player->setScore(this->player->getScore() + 20);
            }
            else
            {
                this->player->setScore(this->player->getScore() + 10);
            }
        }
        else
        {
            obstacle->setVelocity(-10.0f, 0.0f);
            CollisionRect ObstacleCollider = this->obstacles->at(i)->getCollider();
            // Check for collision with player
            raylib::Rectangle Playerrect = {player->getPosition().x + playerCollider.xoffset,
                                            player->getPosition().y + playerCollider.yoffset, playerCollider.width, playerCollider.height};
            raylib::Rectangle ObstacleRect = {obstacle->getPosition().x + ObstacleCollider.xoffset,
                                              obstacle->getPosition().y + ObstacleCollider.yoffset, ObstacleCollider.width, ObstacleCollider.height};
            #if DEBUG == 1
                ObstacleRect.Draw(RED);
                Playerrect.Draw(BLUE);
            #endif
            if (CheckCollisionRecs(Playerrect, ObstacleRect))
            {
                // Handle collision
                this->gameStatus = GAMEOVER;
                this->player->setCurrentAnimation(3); // Set death animation
            }
            else
            {
                obstacle->update();
            }
        }
    }
    if (this->gameStatus == GAMEOVER && this->input->IsActionPerformed(ACTION_JUMP))
    {
        this->SetStatus(RUNNING);
        this->obstacles->clear();
        this->player->setCurrentAnimation(1); // Reset to running animation
        this->player->setPosition(30, 315);   // Reset player position
    }
}

void Game::Draw()
{
    // Draw game elements
    ClearBackground(RAYWHITE);
    if (this->gameStatus == START)
        this->printText("Press Space to Start", 220, 200, 20);
    // Draw Background and paralax
    this->background[0]->Draw((Vector2){-this->backgroundOffset, this->window->GetHeight() - (this->background[0]->GetHeight() * 4.0f)}, 0.0f, 4.0f, WHITE);
    this->background[1]->Draw((Vector2){this->window->GetWidth() - this->backgroundOffset, this->window->GetHeight() - (this->background[1]->GetHeight() * 4.0f)}, 0.0f, 4.0f, WHITE);
    // Draw player
    this->player->Draw();
    for (int i = 0; i < this->obstacles->size(); i++)
    {
        this->obstacles->at(i)->Draw();
    }
    if (this->gameStatus == RUNNING)
    {
        this->backgroundOffset++;
        if (backgroundOffset >= this->background[0]->GetWidth() * 4.0f)
        {
            backgroundOffset = 0; // Reset background offset for parallax effect
        }
    }
    if (this->gameStatus == GAMEOVER)
    {
        this->printText("Game Over; Press Space To Try Again.", 220, 200, 20);
    }
    this->textColor->DrawText(TextFormat("Score: %d", this->player->getScore()), this->window->GetWidth() - 180, 10, 30);

}

void Game::deInit()
{
    delete this->background[0];
    delete this->background[1];
    // De-initialization code
    window->Close();
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
        this->isJumping = false;      // Reset jumping state when on ground
        this->position.y = 315;       // Ensure player stays on ground
        this->setCurrentAnimation(1); // set run animation
    }

    if (input->IsActionPerformed(ACTION_JUMP) && !this->isJumping)
    {
        this->velocity.y = JUMP_FORCE; // Jump velocity
        this->isJumping = true;
        this->setCurrentAnimation(0);
    }

    CollisionRect playerCollider;
    playerCollider.xoffset = 20;
    playerCollider.yoffset = 30;
    playerCollider.width = this->sprite->getCurrentAnimation()->frameWidth - 50;
    playerCollider.height = this->sprite->getCurrentAnimation()->frameHeight - 30;
    setCollider(playerCollider.xoffset, playerCollider.yoffset, playerCollider.width, playerCollider.height);

    if (!isJumping)
    {
        if (input->IsActionPerformed(ACTION_DOWN) && !this->isCrouching)
        {
            this->setCurrentAnimation(2);
            this->position.y += 50;   // Move player up when crouching
            this->isCrouching = true; // Set crouching state
            CollisionRect playerCollider;
            playerCollider.xoffset = 0;
            playerCollider.yoffset = 0;
            playerCollider.width = this->sprite->getCurrentAnimation()->frameWidth;
            playerCollider.height = 42;
            setCollider(playerCollider.xoffset, playerCollider.yoffset, playerCollider.width, playerCollider.height);
        }

        if (isCrouching && input->IsAction_Released(ACTION_DOWN))
        {
            this->isCrouching = false;    // Reset crouching state
            this->position.y -= 50;       // Move player down when standing up
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

EnemyType Obstacle::getType() const
{
    return this->type;
}

Animation loadPlayerRunAnimation()
{
    AnimationBuilder *builder = new AnimationBuilder();
    raylib::Texture2D **textures = new raylib::Texture2D *[2];
    textures[0] = new raylib::Texture2D("resources/run0.png");
    textures[1] = new raylib::Texture2D("resources/run1.png");
    builder->SetTexture(textures);
    builder->SetFrameCount(2);
    builder->SetFrameWidth(90);  // Width of the player sprite
    builder->SetFrameHeight(95); // Height of the player sprite
    builder->SetFrameTime(10);   // Time per frame in milliseconds
    return *builder->Build();
}

Animation loadPlayerCrouchAnimation()
{
    AnimationBuilder *builder = new AnimationBuilder();
    raylib::Texture2D **textures = new raylib::Texture2D *[2];
    textures[0] = new raylib::Texture2D("resources/crouch0.png");
    textures[1] = new raylib::Texture2D("resources/crouch1.png");
    builder->SetTexture(textures);
    builder->SetFrameCount(2);
    builder->SetFrameWidth(118); // Width of the player sprite
    builder->SetFrameHeight(62); // Height of the player sprite
    builder->SetFrameTime(10);   // Time per frame in milliseconds
    return *builder->Build();
}

Animation loadPlayerDeathAnimation()
{
    AnimationBuilder *builder = new AnimationBuilder();
    raylib::Texture2D **textures = new raylib::Texture2D *[1];
    textures[0] = new raylib::Texture2D("resources/death0.png");
    builder->SetTexture(textures);
    builder->SetFrameCount(1);
    builder->SetFrameWidth(90);  // Width of the player sprite
    builder->SetFrameHeight(95); // Height of the player sprite
    builder->SetFrameTime(10);   // Time per frame in milliseconds
    return *builder->Build();
}

Animation loadPetroAnimation()
{
    AnimationBuilder *builder = new AnimationBuilder();
    raylib::Texture2D **textures = new raylib::Texture2D *[2];
    textures[0] = new raylib::Texture2D("resources/petrodactil0.png");
    textures[1] = new raylib::Texture2D("resources/petrodactil1.png");
    builder->SetTexture(textures);
    builder->SetFrameCount(2);
    builder->SetFrameWidth(46);  // Width of the petrodactil sprite
    builder->SetFrameHeight(34); // Height of the petrodactil sprite
    builder->SetFrameTime(10);   // Time per frame in milliseconds
    return *builder->Build();
}

void Game::spawnObstacle()
{
    int width = 0;
    int height = 0;
    int obstacleType = GetRandomValue(0, 3); // Randomly select an obstacle type
    Obstacle *newObstacle = nullptr;
    switch (obstacleType)
    {
    case ENEMY_TYPE_1:
        width = 17;
        height = 55;
        newObstacle = new Obstacle(1, "resources/cactus0.png", 830, 300, width, height, ENEMY_TYPE_1);
        newObstacle->setScaleFactor(4.0f); // Set scale factor for obstacle
        break;
    case ENEMY_TYPE_2:
        width = 17;
        height = 55;
        newObstacle = new Obstacle(2, "resources/cactus1.png", 830, 300, width, height, ENEMY_TYPE_2);
        newObstacle->setScaleFactor(4.0f); // Set scale factor for obstacle
        break;
    case ENEMY_TYPE_3:
        width = 17;
        height = 55;
        newObstacle = new Obstacle(3, "resources/cactus2.png", 830, 300, width, height, ENEMY_TYPE_3);
        newObstacle->setScaleFactor(4.0f); // Set scale factor for obstacle
        break;
    case ENEMY_TYPE_PETRO:
        width = 17;
        height = 35;
        newObstacle = new Obstacle(4, "resources/petrodactil0.png", 830, 300, width, height, ENEMY_TYPE_PETRO);
        newObstacle->addAnimation(loadPetroAnimation());
        newObstacle->setScaleFactor(2.0f); // Set scale factor for obstacle
        newObstacle->setCurrentAnimation(1);
        break;
    }
    newObstacle->setCollider(0, 0, width * 3, height * 2);
    this->obstacles->push_back(newObstacle);
}
