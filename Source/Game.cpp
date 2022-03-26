#include </Users/eavosloo/Programming/C++/sfml_projects/X1/Header/Game.hpp>
#include <Header/Game.hpp>

void Game::initializeVariables(){
    this->window = nullptr;

    // Game logic
    this->endGame = false;
    this->points = 0;
    this->health = 20;
    this->enemySpawnTimerMax = 10.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 10;
    this->mouseHeld = false;

}

void Game::initWindow() {
    this->videoMode.height = 600;
    this->videoMode.width = 800;
    
    this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}

void Game::initEnemies() {
    this->enemy.setPosition(50.f, 50.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(sf::Color::Cyan);
    // this->enemy.setOutlineColor(sf::Color::Green);
    // this->enemy.setOutlineThickness(1.f);
}
// Constructors / Destructors
Game::Game() {
    this->initializeVariables();
    this->initWindow();
    this->initEnemies();
}

Game::~Game() {
    delete this->window;
}

//Accessors
const bool Game::getWindowIsOpen() const{
    return this->window->isOpen();
}
const bool Game::getEndGame() const
{
    return this->endGame;
}


//Functions

void Game::spawnEnemies() 
{
    /*
        @return void
        Spawns enemies sets color and pos.
        -sets a random position
    */
   this->enemy.setPosition(
       static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
       0.f
   );

   this->enemy.setFillColor(sf::Color::Red);

    // spawned enemy
   this->enemies.push_back(this->enemy);
}




void Game::pollEvents() {
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Game::updateMousePositions() 
{
    /**
        -Updates mouse positions
        - vector2i
    */
   this->mousePosWindow = sf::Mouse::getPosition(*this->window);
   this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);

}

void Game::updateEnemies() 
{
    // Updating the timer for enemy spawning
    if(this->enemies.size() < this->maxEnemies)
    {
        if(this->enemySpawnTimerMax >= this->enemySpawnTimer)
        {
            // spawn the enemy and reset
            this->spawnEnemies();
            this->enemySpawnTimer = 0.f;
        }   
        else
            this->enemySpawnTimer += 1.f;
    }
   
    // Move the enemies and updating
    for(int i = 0; i < this->enemies.size(); i++)
    {
        bool deleted = false;

        this->enemies[i].move(0.f, 5.f);

        if(this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
            std::cout << "Health: " << this->health << std::endl;
        }
            
        
    }

    // Check if clicked

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if(this->mouseHeld == false) 
        {
            this->mouseHeld = true;
            bool deleted = false;

            for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if(this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    // Delete the enemy
                    deleted = true;

                    this->enemies.erase(this->enemies.begin() + i);

                    // Gain points
                    this->points += 1;
                    std::cout << "Points: " << this->points << std::endl;
                }
            }          
        }
    }
    else{
        this->mouseHeld = false;
    }


}



void Game::update() {

    this->pollEvents();

    if(!this->endGame)
    {
        this->updateMousePositions();

        this->updateEnemies();
    }

    if(this->health <= 0)
        this->endGame = true;


}

void Game::renderEnemies()
{
    for(auto &e : this->enemies)
    {
       this->window->draw(e);
    }
    
}

void Game::render() {
    /*
        Render the game objects
    */
    this->window->clear();

    // Draw game objects
    this->renderEnemies();
    
  
    this->window->display();
}