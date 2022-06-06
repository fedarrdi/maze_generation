#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
const int WIDTH = 800, HEIGHT = 800;

struct Point { double x, y; };

double dist(Point p, Point q)
{
    double rx = p.x - q.x;
    double ry = p.y - q.y;
    return sqrt(rx * rx + ry * ry);
}

Point move_point(Point p, Point q, double u)
{
    double d = dist(p, q);
    double dt = d * u;
    double t = dt / d;
    Point pt = {(1 - t) * p.x + t * q.x, (1 - t) * p.y + t * q.y};
    return pt;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
        
    Point a = {500, 700}, b = {100, 300}, c = {500, 100};
    
    sf::CircleShape s1(5);
    s1.setPosition(a.x, a.y);
    sf::CircleShape s2(5);
    s2.setPosition(b.x, b.y);
    sf::CircleShape s3(5);
    s3.setPosition(c.x, c.y);

    sf::Vertex l1[] =
    {
        sf::Vertex(sf::Vector2f(a.x, a.y)),
        sf::Vertex(sf::Vector2f(b.x, b.y))
    };

    sf::Vertex l2[] =
    {
        sf::Vertex(sf::Vector2f(b.x, b.y)),
        sf::Vertex(sf::Vector2f(c.x, c.y))
    };

    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(s1);
        window.draw(s2);
        window.draw(s3);
        window.draw(l1, 2, sf::Lines);
        window.draw(l2, 2, sf::Lines);
    
        for(double t = 0;t <= 1; t += 0.01)
        {
            Point curr_point1 = move_point(a, b, t);
            sf::CircleShape p1(2);        
            p1.setPosition(curr_point1.x, curr_point1.y);
            window.draw(p1); 

            Point curr_point2 = move_point(b, c, t);
            sf::CircleShape p2(2);        
            p2.setPosition(curr_point2.x, curr_point2.y);
            window.draw(p2);
    
            sf::Vertex ld[] =
            {   
                sf::Vertex(sf::Vector2f(curr_point1.x, curr_point1.y)),
                sf::Vertex(sf::Vector2f(curr_point2.x, curr_point2.y))
            };

            Point bizie_point = move_point(curr_point1, curr_point2, t);
            sf::CircleShape p3(2);        
            p3.setPosition(bizie_point.x, bizie_point.y);
            window.draw(p3);
    
            Point mid_point = { (curr_point1.x + curr_point2.x) / 2, (curr_point1.y + curr_point2.y) / 2};

            sf::CircleShape p4(2);
            p4.setPosition(mid_point.x, mid_point.y);
            window.draw(p4);

            //window.draw(ld, 2, sf::Lines);
    

        }

        window.display();
    }

    return 0;
}
