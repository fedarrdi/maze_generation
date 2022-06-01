#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

struct point { double x, y; };

struct line { point a, b; };

struct triangle{ point a, b, c; };

double dist(point a, point b)
{
    double rx = a.x - b.x, ry = a.y - b.y;
    return sqrt (rx * rx + ry * ry);
}

triangle set_triangle(point A, point B)
{
    triangle curr;
    double length = dist(A, B), angle = 61.78;   // 61.78;

    point C = {cos(angle) * length + A.x, sin(angle) * length + A.y};    
    curr.a = A, curr.b = B, curr.c = C;

    return curr;
}

double area_of_triangle(point a, point b, point c)
{
    return abs( a.x*( b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) ) / 2;
}

bool is_Point_in_triangle(triangle p, point q)
{
    double s1 = area_of_triangle(p.a, p.c, q), 
           s2 = area_of_triangle(p.a, p.b, q),
           s3 = area_of_triangle(p.b, p.c, q),
           s =  area_of_triangle(p.a, p.b, p.c);
    
    return (s1 + s2 + s3 > s) ? 0 : 1;
}

const int WIDTH = 800, HEIGHT = 800;

point random_point_in_triangle(triangle t)
{
    point tmp;
    do
    {
        tmp.x = rand() % WIDTH, tmp.y = rand() % HEIGHT;
    }
    while(!is_Point_in_triangle(t, tmp));

    return tmp;
}


point find_mid_point_line(line l)
{
    double m = (l.b.y - l.a.y) / (l.b.x - l.a.x);
    double b = l.b.y - m * l.b.x;
    double x = abs(l.b.x + l.a.x) / 2;
    double y = m * x + b;
    return (point) { x, y };
}


vector<point> draw_points_in_triangle(triangle t, point start_point)
{
    int point_count = 100000;
    vector<point> out;

    for(int i = 0;i < point_count;i++)
    {
        int j = rand() % 3;
        
        point end_point;

        if(j == 0) end_point = t.a;
        if(j == 1) end_point = t.b;
        if(j == 2) end_point = t.c;

        out.push_back(start_point);
        point new_start_point = find_mid_point_line({start_point, end_point});
        start_point = new_start_point;
    }
    
    return out;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

    srand(static_cast<unsigned int> (time(nullptr)));

    point a = {10, 790}, b = {790, 790};
    
    triangle t = set_triangle(a, b); 
    sf::Vertex line[] { sf::Vertex(sf::Vector2f(t.a.x, t.a.y)), sf::Vertex(sf::Vector2f(t.b.x, t.b.y)) };
    sf::Vertex line1[] { sf::Vertex(sf::Vector2f(t.a.x, t.a.y)), sf::Vertex(sf::Vector2f(t.c.x, t.c.y))};
    sf::Vertex line2[] { sf::Vertex(sf::Vector2f(t.b.x, t.b.y)), sf::Vertex(sf::Vector2f(t.c.x, t.c.y))};
 
    point p = random_point_in_triangle(t);  
           
    vector<point> points = draw_points_in_triangle(t, p);
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        



        window.clear();
        window.draw(line, 2, sf::Lines);
        window.draw(line1, 2, sf::Lines);
        window.draw(line2,   2, sf::Lines);
        
        for(auto curr_pos : points)
        {
            sf::CircleShape shape(2);
            shape.setPosition(curr_pos.x, curr_pos.y);
            window.draw(shape);
        }


        window.display();
    }

    return 0;
}
