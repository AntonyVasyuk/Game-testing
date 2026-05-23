#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <math.h>


#define WIDTH 800
#define HEIGHT 800

//float G = 6.67430 * std::pow(10, -11);
//int WARP = 4 * 10000;

float G = 6.67430;
int WARP = 100000;
int SCALE = 1e7 / 4;


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
    void attract_to_object(Object& other);
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

void Object::add_velocity(Object& other, float dv)
{
    float d = distance_between_objects(*this, other);

    float cos = (other.get_coordinates().first - x) / d;
    float sin = (other.get_coordinates().second - y) / d;

    vx += dv * cos;
    vy += dv * sin;
}

void Object::attract_to_object(Object& other)
{
    float dv = (G * other.get_mass()) * 100000 / std::pow(distance_between_objects(*this, other), 2);
    this->add_velocity(other, dv * WARP);
    //std::cout << dv * WARP << '\n';
}

void Object::update()
{
    this->x += this->vx * WARP;
    this->y += this->vy * WARP;
}

//end of Object functions


class Camera : public Object
{
private:
    float dx;
    float dy;
    float rotation;
    int scale;

public:
    Camera(float x, float y, float dx, float dy, float rotaion, int scale);
    std::pair<float, float> get_projection(Object& other);
    int get_scale() { return this->scale; }
    std::pair<float, float> get_delta() { return std::pair<float, float>(dx, dy); }
    void move(float dx, float dy);
};


//Planet class definition

class Planet : public Object
{
private:
    int radius;
    std::tuple<short, short, short> color;

public:
    Planet(float x, float y, float vx, float vy, int mass, int radius, std::tuple<short, short, short> color);

    std::tuple<short, short, short> get_color();

    void draw(sf::RenderWindow& window, Camera camera);
};


//Planet class functions

Planet::Planet(float x, float y, float vx, float vy, int mass, int radius, std::tuple<short, short, short> color) : Object(x, y, vx, vy, mass)
{
    this->radius = radius;

    this->color = color;
}

std::tuple<short, short, short> Planet::get_color() { return this->color; }

void Planet::draw(sf::RenderWindow& window, Camera camera)
{
    int scale = camera.get_scale();

    sf::CircleShape circle(radius / scale + 10);

    auto c = camera.get_projection(*this);
    circle.setPosition({ c.first, c.second });

    //std::cout << radius / scale << '\n';

    auto color = this->get_color();

    circle.setFillColor(sf::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color)));
    //circle.setFillColor(sf::Color(255, 255, 255));
    //sf::Color()

    //std::cout << '1';

    window.draw(circle);
}

// end of Planet functions


Camera::Camera(float x, float y, float dx, float dy, float rotaion, int scale) : Object(x, y, 0, 0, 0)
{
    this->dx = dx;
    this->dy = dy;
    this->rotation = rotaion;
    this->scale = scale;
}

std::pair<float, float> Camera::get_projection(Object &other)
{
    float cosa = std::cos(this->rotation), sina = std::sin(this->rotation);

    auto cc = this->get_coordinates();
    auto c1 = other.get_coordinates();
    float x2 = c1.first - cc.first, y2 = c1.second - cc.second;

    x2 = x2 * cosa - y2 * sina;
    y2 = x2 * sina + y2 * cosa;

    int scale = this->get_scale();
    x2 /= scale;
    y2 /= scale;

    auto d = this->get_delta();
    x2 = x2 + d.first;
    y2 = y2 + d.second;

    return std::pair<float, float>(x2, y2);
}

void Camera::move(float dx, float dy)
{
    //this
}


int main()
{
    sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "First time");

    window.setFramerateLimit(24);

    std::tuple<short, short, short> c1, c2;
    c1 = std::make_tuple(50, 70, 219);
    c2 = std::make_tuple(217, 213, 206);

    Camera camera(0, 0, (float)WIDTH / 2, (float)HEIGHT / 2, 0, SCALE);

    Planet a(0, 0, 0, 0, 5.972e8, 6378e3, c1), b(384399e3, 0, 0, -1022, 7.346e6, 1737e3, c2);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            //if (event->is<sf::Event::KeyPressed>())
            //    camera.
        }

        window.clear();

        //a.attract_to_object(b);
        b.attract_to_object(a);

        //a.update();
        b.update();

        a.draw(window, camera);
        b.draw(window, camera);

        window.display();

    }
}


float distance_between_objects(Object a, Object b)
{
    auto xa = a.get_coordinates(), xb = b.get_coordinates();
    return std::sqrt(std::pow(xa.first - xb.first, 2) + std::pow(xa.second - xb.second, 2));
}
