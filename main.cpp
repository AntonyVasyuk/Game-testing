#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <math.h>


#define WIDTH 800
#define HEIGHT 800

//float G = 6.67430 * std::pow(10, -11);
//int WARP = 4 * 10000;

float G = 10;
int WARP = 1;


class Object
{
private:
    float x;
    float y;
    float vx;
    float vy;
    int mass;

public:
    Object(float x, float y, float vx, float vy, int mass);
    std::pair<float, float> get_coordinates();
    int get_mass();
    void add_velocity(float dvx = 0, float dvy = 0);
    void add_velocity(Object& other, float dv = 0);
    void update();
};


float distance_between_objects(Object a, Object b);


//Object functions

Object::Object(float x, float y, float vx, float vy, int mass)
{
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->mass = mass;
}

std::pair<float, float> Object::get_coordinates() { return std::pair<float, float>(x, y); }

int Object::get_mass() { return this->mass; }

void Object::add_velocity(float dvx, float dvy)
{
    vx += dvx;
    vy += dvy;
}

void Object::add_velocity(Object &other, float dv)
{
    float d = distance_between_objects(*this, other);

    float cos = (other.get_coordinates().first - x) / d;
    float sin = (other.get_coordinates().second - y) / d;

    vx += dv * cos;
    vy += dv * sin;
}

void Object::update()
{
    this->x += this->vx * WARP;
    this->y += this->vy * WARP;
}

//end of Object functions


//Planet class definition

class Planet: public Object
{
private:
    int radius;
    std::tuple<short, short, short> color;
    
public:
    Planet(float x, float y, float vx, float vy, int mass, int radius, std::tuple<short, short, short> color): Object(x, y, vx, vy, mass)
    {
        this->radius = radius;
        this->color = color;
    }

    void Attract_to_object(Object &other)
    {
        float dv = (G * other.get_mass()) / std::pow(distance_between_objects(*this, other), 2);
        this->add_velocity(other, dv * WARP);
    }

    std::tuple<short, short, short> get_color() { return this->color; }

    void draw(sf::RenderWindow &window)
    {
        sf::CircleShape circle(radius);

        auto c = this->get_coordinates();
        circle.setPosition({ (float)c.first, (float)c.second });

        auto color = this->get_color();

        circle.setFillColor(sf::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color)));
        //circle.setFillColor(sf::Color(255, 255, 255));

        //std::cout << '1';

        window.draw(circle);
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "First time");

    std::tuple<short, short, short> c1, c2;
    c1 = std::make_tuple(255, 255, 255);
    c2 = std::make_tuple(0, 255, 255);

    Planet a(WIDTH / 2, HEIGHT / 2, 0, 0, 1, 20, c1), b(WIDTH / 2 + 200, HEIGHT / 2, 0, 0, 1, 10, c2);

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

        a.update();
        b.update();

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
