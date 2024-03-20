#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#include <Helper.h>

//////////////////////////////////////////////////////////////////////
/// NOTE: this include is needed for environment-specific fixes     //
/// You can remove this include and the call from main              //
/// if you have tested on all environments, and it works without it //
#include "env_fixes.h"                                              //
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// This class is used to test that the memory leak checks work as expected even when using a GUI
class Entity
{
    public:

    Entity(float x, float y) {
    _startingPosition.x = x;
    _startingPosition.y = y;
    _health = 1;
    _movementSpeed = 0.2;
    _sprite.setPosition(_startingPosition);
    }
    Entity(float x, float y, int health, float movementSpeed)
    {
        _startingPosition.x = x;
        _startingPosition.y = y;
        _health = health;
        _movementSpeed = movementSpeed;
        _sprite.setPosition(_startingPosition);
    }
    ~Entity()
    {
        std::cout << "enitity removed";
    }


        //GETTERS
        int getHealth()
{
    return _health;
}

float getMovementSpeed()
{
    return _movementSpeed;
}

sf::Sprite &getSprite()
{
    return _sprite;
}

sf::Vector2f getStartingPosition()
{
    return _startingPosition;
}


        //SETTERS
      void setTexture(std::string name)
{
    if(!_texture.loadFromFile(name))
    {
        std::cerr << "ERROR, CANNOT LOAD TEXTURE" << std::endl;
        EXIT_FAILURE;
    }
    else
    {
        _sprite.setTexture(_texture);
    }
}


void setHealth(int value)
{
    _health = value;
}



        //FUNCTIONS
       void movement(sf::Time frameTime, short int direction)
{
    switch(direction)
    {
        case 1: //MOVE LEFT
           _sprite.move(-1 * _movementSpeed * frameTime.asMilliseconds(), 0);
           break;
        case 2: //MOVE RIGHT
           _sprite.move(_movementSpeed * frameTime.asMilliseconds(), 0);
           break;
        case 3: // MOVE UP
           _sprite.move(0, -1 * _movementSpeed * frameTime.asMilliseconds());
           break;
        case 4: //MOVE DOWN
           _sprite.move(0, _movementSpeed * frameTime.asMilliseconds());
           break;
    }
}
bool isDead()
{
    if(_health <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void hpDown()
{
    _health = _health - 1;
    std::cout << "HP DOWN = " << _health << std::endl;
}


friend std::ostream & operator << (std::ostream & out , const Entity & e);
    protected:
        sf::Vector2f _startingPosition;
        sf::Texture _texture;
        sf::Sprite _sprite;
        float _movementSpeed;
        int _health;

    private:
};


class Player : public Entity
{
    public:
       Player(float x, float y) : Entity(x, y, 3, 0.5f)
{
    setTexture("player.png");
}

~Player()
{

}
};

class Bullet
{
    public:


Bullet()
{
    _rect.setSize(sf::Vector2f(3,5));
    _movementSpeed = 0.5f;
    _color = sf::Color::White;
    _direction = 1;
}

Bullet(float x, float y, short int direction, sf::Color color)
{
    _rect.setSize(sf::Vector2f(3,5));
    _rect.setPosition(x, y);
    _movementSpeed = 0.5f;
    _color = color;
    _direction = direction;
}

Bullet(float x, float y, short int direction, sf::Color color, float movementSpeed)
{
    _rect.setSize(sf::Vector2f(3,5));
    _rect.setPosition(x, y);
    _movementSpeed = movementSpeed;
    _color = color;
    _direction = direction;
}
Bullet(Bullet &t){
    _rect.setSize(sf::Vector2f(t.getRect().getOrigin()));
    _rect.setPosition(x, y);
    _movementSpeed = movementSpeed;
    _color = color;
    _direction = direction;
}

~Bullet()
{

}

//GETTERS

float getMovementSpeed()
{
    return _movementSpeed;
}

short int getDirection()
{
    return _direction;
}

sf::RectangleShape getRect()
{
    return _rect;
}

//FUNCTIONS

void movement(sf::Time frameTime)
{
    switch(_direction)
    {
        case 1: //MOVE UP
           _rect.move(0, -1 * _movementSpeed * frameTime.asMilliseconds());
           break;
        case 2: //MOVE DOWN
           _rect.move(0, _movementSpeed * frameTime.asMilliseconds());
           break;
    }
}
friend std::ostream & operator << (std::ostream & out , const Bullet & e);

    protected:
        float _movementSpeed;
        sf::RectangleShape _rect;
        sf::Color _color;
        short int _direction; // 1-UP 2-DOWN


    private:
};

std::ostream & operator << (std::ostream & out , const Bullet & e){
    out<<e._movementSpeed  << " " << e._direction<<" ";
    return out;
}
std::ostream & operator << (std::ostream & out , const Entity & e){
    out<<e._movementSpeed <<" " <<e._health<<" ";
    return out;
}
////////////////////////////////////////////////////////////////////////


int main() {
    ////////////////////////////////////////////////////////////////////////
    /// NOTE: this function call is needed for environment-specific fixes //
    init_threads();                                                       //
    ////////////////////////////////////////////////////////////////////////
    ///
   
    /////////////////////////////////////////////////////////////////////////
    /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    /// dați exemple de date de intrare folosind fișierul tastatura.txt
    /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    /// pentru cât mai multe ramuri de execuție.
    /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    ///
    /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    /// pentru a simula date introduse de la tastatură.
    /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    ///
    /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    /// program care merg (și să le evitați pe cele care nu merg).
    ///
    /////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////////////
    /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
    /// alt fișier propriu cu ce alt nume doriți.
    /// Exemplu:
    /// std::ifstream fis("date.txt");
    /// for(int i = 0; i < nr2; ++i)
    ///     fis >> v2[i];
    ///
    ///////////////////////////////////////////////////////////////////////////
    ///                Exemplu de utilizare cod generat                     ///
    ///////////////////////////////////////////////////////////////////////////
    Helper helper;
    helper.help();
    ///////////////////////////////////////////////////////////////////////////


    sf::RenderWindow window;
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "My Window", sf::Style::Default);
    ///////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                            ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////
    float x,y,gx,gy;
    std::cin>>x>>y >> gx>>gy;
    Bullet b(gx,gy,0,"red",0);
    Player p(x,y);
    b.getRect();
    b.getDirection();
    b.getMovementSpeed();
    p.getHealth();
    p.getMovementSpeed();
    p.getSprite();
    p.getStartingPosition();
    p.hpDown();
    p.isDead();
    p.movement();
    p.setHealth(1);
    std::cout<<p<< " "<< b;
    while(window.isOpen()) {
        bool shouldExit = false;
        sf::Event e{};
        while(window.pollEvent(e)) {
            switch(e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                std::cout << "New width: " << window.getSize().x << '\n'
                          << "New height: " << window.getSize().y << '\n';
                break;
            case sf::Event::KeyPressed:
                std::cout << "Received key " << (e.key.code == sf::Keyboard::X ? "X" : "(other)") << "\n";
                if(e.key.code == sf::Keyboard::Escape)
                    shouldExit = true;
                break;
            default:
                break;
            }
        }
        if(shouldExit) {
            window.close();
            break;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(300ms);

        window.clear();
        window.display();
    }
    return 0;
}
