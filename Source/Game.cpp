#include </Users/eavosloo/Programming/C++/sfml_projects/BlockClicker/Header/Game.hpp>
//#include <Header/Game.hpp>

void Game::initializeVariables(){
    this->window = nullptr;

    // Game logic
    this->endGame = false;
    this->points = 0;
    this->health = 25;
    this->enemySpawnTimerMax = 30.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 6;
    this->mouseHeld = false;

}

void Game::initWindow() {
    this->videoMode.height = 600;
    this->videoMode.width = 800;
    
    this->window = new sf::RenderWindow(this->videoMode, "Block Clicker", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}


void Game::initFonts() {
    if(this->font.loadFromFile("/Users/eavosloo/Programming/C++/sfml_projects/BlockClicker/Fonts/Dosis-Light.ttf"))
    {
        std::cout << "ERROR::GAME::INITFONTS::Failed to load fonts";
    }
}


void Game::initText() {
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}


void Game::initEnemies() {
    this->enemy.setPosition(50.f, 50.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setFillColor(sf::Color::Cyan);
}


// Constructors / Destructors
Game::Game() {
    this->initializeVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
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

        Spawns enemies & sets color, types and posistions randomly.
        -sets a random position
    */
   this->enemy.setPosition(
       static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
       0.f
   );

   // Randomize enemy type
   int type = rand() % 5;

   switch (type)
   {
   case 0:
       this->enemy.setFillColor(sf::Color::Magenta);
       this->enemy.setSize(sf::Vector2f(10.f, 10.f));
       break;
    case 1:
       this->enemy.setFillColor(sf::Color::Blue);
       this->enemy.setSize(sf::Vector2f(30.f, 30.f));
       break;
    case 2:
       this->enemy.setFillColor(sf::Color::Cyan);
       this->enemy.setSize(sf::Vector2f(50.f, 50.f));
       break;
    case 3:
       this->enemy.setFillColor(sf::Color::Red);
       this->enemy.setSize(sf::Vector2f(70.f, 70.f));
       break;
    case 4:
       this->enemy.setFillColor(sf::Color::Green);
       this->enemy.setSize(sf::Vector2f(100.f, 100.f));
       break;
   default:
       this->enemy.setFillColor(sf::Color::Yellow);
       this->enemy.setSize(sf::Vector2f(100.f, 100.f));
       break;
   }

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

void Game::updateText() {
    std::stringstream ss;
    
    ss << "Points: " << this->points << '\n'
        << "Health: " << this->health << '\n';

    this->uiText.setString(ss.str());

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

                    // Gain points
                    if(this->enemies[i].getFillColor() == sf::Color::Magenta)
                        this->points += 10;
                    else if(this->enemies[i].getFillColor() == sf::Color::Blue)
                        this->points += 7;
                    else if(this->enemies[i].getFillColor() == sf::Color::Cyan)
                        this->points += 5;
                    else if(this->enemies[i].getFillColor() == sf::Color::Red)
                        this->points += 3;
                    else if(this->enemies[i].getFillColor() == sf::Color::Green)
                        this->points += 1;

                    std::cout << "Points: " << this->points << std::endl;

                    // Delete the enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
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

        this->updateText();

        this->updateEnemies();
    }

    if(this->health <= 0)
        this->endGame = true;


}

void Game::renderText(sf::RenderTarget& target) {
    target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    for(auto &e : this->enemies)
    {
       target.draw(e);
    }
    
}

void Game::render() {
    /*
        Render the game objects
    */
    this->window->clear();

    // Draw game objects
    this->renderEnemies(*this->window);

    this->renderText(*this->window);
    
    this->window->display();
}