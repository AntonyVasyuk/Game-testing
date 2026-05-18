#include <SFML/Graphics.hpp>


class Object
{
    private
};


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "First time", sf::State::Fullscreen);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}