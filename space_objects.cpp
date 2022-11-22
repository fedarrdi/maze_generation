#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
using namespace std;

const int WIDTH = 800, HEIGHT = 800;

class Vector2D
{

private:
    float x, y;

public:
    Vector2D() {}

    Vector2D(float x, float y) : x(x), y(y) { }
    
    void set_x(float x) { this->x = x; }
    void set_y(float y) { this->y = y; }

    float get_x() const { return x; }
    float get_y() const { return y; }

    float get_magnitude() { return sqrt(x*x + y*y); }

    Vector2D get_unit_vector()
    {
        float magnitude = get_magnitude(); 
        return Vector2D(x / magnitude, y / magnitude);
    }

    Vector2D operator + (const Vector2D &other)
    {
        return Vector2D(x + other.get_x(), y + other.get_y());
    }

    Vector2D operator + (float num)
    {
        return Vector2D(x + num, y + num);
    }
    
    Vector2D operator - (const Vector2D &other)
    {
        return Vector2D(x - other.get_x(), y - other.get_y());
    }

    Vector2D operator - (float num)
    {
        return Vector2D(x - num, y - num);
    }

    Vector2D operator / (float num)
    {
        return Vector2D(x / num, y / num);
    }
    
    Vector2D operator * (float num)
    {
        return Vector2D(x * num, y * num);
    }

    Vector2D operator = (const Vector2D &other)
    {
        x = other.get_x();
        y = other.get_y();
        return *this;
    }

    void print()
    {
        cout << "x: " << x << " y: " << y << endl;
    }
};

class SpaceObject
{

private:
    string name;
    float mass, r;
    Vector2D position, acceleration, speed;

public:
    SpaceObject() {}
    
    SpaceObject(string name, float mass, float r, Vector2D position)
    {
        this->name = name;
        this->mass = mass;
        this->r = r;
        this->position = position;
        acceleration = Vector2D(0, 0);
        speed = Vector2D(0, 0);
    }

    SpaceObject(string name, float mass, float r, float x, float y)
    {
        this->name = name;
        this->mass = mass;
        this->r = r;
        position = Vector2D(x, y);
        acceleration = Vector2D(0, 0);
        speed = Vector2D(0, 0);
    }

    Vector2D get_position() const { return position; }
    float get_mass() const { return mass; } 
    string get_name() const { return name; }

    void print()
    {   
        cout << "Name: " << name << endl;
        cout << "Mass: " << mass << endl;
        cout << "Position: ";
        position.print();
        cout << "Acceleration: ";
        acceleration.print();
        cout << "Speed: ";
        speed.print();
    }

    Vector2D calculate_force(const SpaceObject &other)
    {   
        Vector2D v = position - other.get_position();
        float dist = v.get_magnitude();
        float F = (mass * other.get_mass()) / (dist * dist);
        Vector2D direction_times_force = v.get_unit_vector() * F;
        return direction_times_force;
    }

    void calculate_acceleration(const SpaceObject &other)
    {
        Vector2D new_acceleration = calculate_force(other) / mass;
        acceleration = acceleration + new_acceleration;
    }

    void calculate_speed(const SpaceObject &other)
    {
        calculate_acceleration(other);
        speed = speed + acceleration;
        cout << name << endl;
        speed.print();
        cout << endl;
    }

    void move()
    {
        position = position + speed;
    }

    sf::CircleShape get_draw_obj()
    {
        sf::CircleShape shape(r);
        shape.setPosition(sf::Vector2f(position.get_x(), position.get_y()));
        return shape;
    }
};

class Space
{
private:
    vector<SpaceObject> objects;

public:
    Space() {}

    void add_object(SpaceObject new_obj)
    {
        objects.push_back(new_obj);
    }

    void move_objects()
    {
        for(auto curr_obj : objects)
            for(auto other_obj : objects)
                if(curr_obj.get_name() != other_obj.get_name())
                    curr_obj.calculate_speed(other_obj);   
        
        for(auto curr_obj : objects)
        {
            //curr_obj.print();
            curr_obj.move();
        }
    }

    vector<sf::CircleShape> get_draw_objs()
    {
        vector<sf::CircleShape> out;
        for(auto curr_obj : objects)
            out.push_back(curr_obj.get_draw_obj());
        return out;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
    Space s = Space();   
    SpaceObject s1 = SpaceObject("A", 100000, 20, 300, 300);
    SpaceObject s2 = SpaceObject("B", 100, 20, 100, 100);
    s.add_object(s1);
    s.add_object(s2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        s.move_objects();
        vector<sf::CircleShape> objs = s.get_draw_objs();
        
        window.clear();
        
        for(auto curr : objs)
            window.draw(curr);
        
        window.display();
    }

    return 0;
}
