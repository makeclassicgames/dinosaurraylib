#include <raylib-cpp.hpp>

#include "game.hpp"

int main() {
    
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    
    Game game;

    game.Init(screenWidth, screenHeight);
   
    
 

    // Main game loop
    while (!game.isWindowClosed()) // Detect window close button or ESC key
    {
        // Update
        game.Update();
        // TODO: Update your variables here

        // Draw
        BeginDrawing();
        game.Draw();
        EndDrawing();
    }
    game.deInit();
     // Close window and OpenGL context
    return 0;
}