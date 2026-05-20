#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <math.h>


#define WIDTH 800
#define HEIGHT 800
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
    Object(int x, int y, int vx, int vy, int mass);
    std::pair<int, int> get_coordinates();
    int get_mass();
    void add_velocity(int dvx = 0, int dvy = 0);
    void add_velocity(Object& other, float dv = 0);
};


float distance_between_objects(Object a, Object b);


Object::Object(int x, int y, int vx, int vy, int mass)
{
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->mass = mass;
}

std::pair<int, int> Object::get_coordinates() { return std::pair<int, int>(x, y); }

int Object::get_mass() { return this->mass; }

void Object::add_velocity(int dvx, int dvy)
{
    vx += dvx;
    vy += dvy;
}

void Object::add_velocity(Object &other, float dv)
{
    float d = distance_between_objects(*this, other);

    float cos = (x - other.get_coordinates().first) / d;
    float sin = (y - other.get_coordinates().second) / d;

    vx += dv * cos;
    vy += dv * sin;
}


class Planet: public Object
{
private:
    int radius;
    std::tuple<int, int, int> color;
    
public:
    Planet(int x, int y, int vx, int vy, int mass, int radius, std::tuple<int, int, int> color): Object(x, y, vx, vy, mass)
    {
        this->radius = radius;
        this->color = color;
    }

    void Attract_to_object(Object &other)
    {
        float dv = (G * other.get_mass()) / std::pow(distance_between_objects(*this, other), 2);
        this->add_velocity(other, dv);
    }

    std::tuple<int, int, int> get_color() { return this->color; }

    void draw(sf::RenderWindow &window)
    {
        sf::CircleShape circle(radius);

        auto c = this->get_coordinates();
        circle.setPosition({ (float)c.first, (float)c.second });

        auto color = this->get_color();

        circle.setFillColor(sf::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color)));
        //circle.setFillColor(sf::Color(255, 255, 255));

        std::cout << '1';

        window.draw(circle);
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "First time");

    std::tuple<int, int, int> c1, c2;
    c1 = std::make_tuple(255, 255, 255);
    c2 = std::make_tuple(0, 255, 255);

    Planet a(WIDTH / 2, HEIGHT / 2, 0, 0, 10, 20, c1), b(WIDTH / 2 + 50, HEIGHT / 2, 0, -1, 1, 10, c2);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        b.Attract_to_object(a);

        a.draw(window);
        b.draw(window);

        window.display();

    }
}


float distance_between_objects(Object a, Object b)
{
    auto xa = a.get_coordinates(), xb = b.get_coordinates();
    return std::sqrt(std::pow(xa.first - xb.first, 2) + std::pow(xa.second - xb.second, 2));
}
