#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "World.hpp"
#include <iostream>
#include <cmath>

using namespace sf;

// Weapon
// Bots
// Gameplay

int main()
{
    RenderWindow window(VideoMode(1920, 1080), "New DOOM");
    window.setMouseCursorVisible(false);
    window.setVerticalSyncEnabled(true);
    Mouse::setPosition({ (int)window.getSize().x / 2, (int)window.getSize().y / 2 }, window);
    
    World game(window, "resources/maze.txt");
    Clock clock;
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds(); 
        game.curFps.update(floor(1.0f / clock.restart().asSeconds()));

        Event event;
        while (window.pollEvent(event))
        {
            if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::M) 
                game.setFlagMiniMap(!game.getFlagMiniMap());
        }

        window.clear(colorSky);
        game.updateWorld(window, time);
        game.showWorld(window);
        window.display();
    }
    return 0;
}
