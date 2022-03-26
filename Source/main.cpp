#include </Users/eavosloo/Programming/C++/sfml_projects/BlockClicker/Header/Game.hpp>
#include <Header/Game.hpp>

int main()
{
    //Init srand
    std::srand(static_cast<unsigned>(time(NULL)));
    //Init Game Engine
    Game game;

    // Game loop
    while (game.getWindowIsOpen() && !game.getEndGame())
    {
        // Update
        game.update();
        // Render
        game.render();
    }

    // End of Game
    return 0;
}