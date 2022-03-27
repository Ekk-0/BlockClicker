#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

/*
    Class that acts as the game engine.
    Wrapper Class
*/

class Game
{
    public:
        //Constructors / Destructor
        Game();
        virtual ~Game();
        // Accessors
        const bool getWindowIsOpen() const;
        const bool getEndGame() const;

        // Functions
        void spawnEnemies();

        void pollEvents();
        void updateMousePositions();
        void updateText();
        void updateEnemies();
        void update();

        void renderText(sf::RenderTarget& target);
        void renderEnemies(sf::RenderTarget& target);
        void render();

    private:
    //Variables
    // Window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

    // Mouse Positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    // Resources
    sf::Font font;

    // Text
    sf::Text uiText;

    // Game logic
    bool endGame;
    unsigned points;
    int health;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;

    // Game objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;
    

    // Private functions
    void initializeVariables();
    void initWindow();
    void initFonts();
    void initText();
    void initEnemies();

};

#endif