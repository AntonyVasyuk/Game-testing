#include <SFML/Graphics.hpp>
#include <math.h>


#define G 10


class Object
{
private:
    int x;
    int y;
    int vx;
    int vy;
    int mass;

public:
    Object(int x, int y, int vx, int vy)
    {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
    }

    std::pair<int, int> get_coordinates() { return std::pair<int, int>(x, y); }

    int get_mass() { return this->mass; }

    void add_velocity(int dvx=0, int dvy=0)
    {
        vx += dvx;
        vy += dvy;
    }

    void add_velocity(Object &other, float dv = 0)
    {


        vx += dvx;
        vy += dvy;
    }

};


float distance_between_objects(Object a, Object b);


class Planet: public Object
{
private:
    int radius;
    
public:
    void Attract_to_object(Object &other)
    {
        float dv = (G * other.get_mass()) / std::pow(distance_between_objects(*this, other), 2);
        this->add_velocity(other, dv);
    }
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


float distance_between_objects(Object a, Object b)
{
    auto xa = a.get_coordinates(), xb = b.get_coordinates();
    return std::sqrt(std::pow(xa.first - xb.first, 2) + std::pow(xa.second - xb.second, 2));
}
