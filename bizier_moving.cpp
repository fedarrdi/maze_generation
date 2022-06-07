#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <vector>


using namespace std;

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

Point get_player_delta(Point player, Point destination)
{
    return (Point) {(destination.x - player.x) / 100, (destination.y - player.y) / 100 };
}


vector<Point> generate_bezier_curve(Point a, Point b, Point c, double t_step)
{
    vector<Point> bezier_curve;
    for(double t = 0;t <= 1;t += t_step)
    {
        Point curr_point1 = move_point(a, b, t);
        Point curr_point2 = move_point(b, c, t);
        Point bezier_point = move_point(curr_point1, curr_point2, t);
        bezier_curve.push_back(bezier_point);
    }
    return bezier_curve;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
        
    Point player = {500, 700}, b = {100, 300}, c = {500, 100}, delta;

    sf::CircleShape s1(5);
    s1.setPosition(player.x, player.y);
    sf::CircleShape s2(5);
    s2.setPosition(b.x, b.y);
    sf::CircleShape s3(5);
    s3.setPosition(c.x, c.y);

    sf::Vertex l1[] ={sf::Vertex(sf::Vector2f(player.x, player.y)),sf::Vertex(sf::Vector2f(b.x, b.y))};
    sf::Vertex l2[] ={sf::Vertex(sf::Vector2f(b.x, b.y)),sf::Vertex(sf::Vector2f(c.x, c.y))};

    vector<Point> bezier_curve = generate_bezier_curve(player, b, c, 0.01);
    int last_bizier_point_index = 0;
        
    sf::CircleShape p(5);
    p.setPosition(player.x, player.y);

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
    
        for(auto c_p_b : bezier_curve)
        {
            sf::CircleShape s_b(2);
            s_b.setPosition(c_p_b.x, c_p_b.y);
            window.draw(s_b);
        }
        
        if(dist(player,  bezier_curve[last_bizier_point_index]) < 0.05) 
        {
            last_bizier_point_index++;
            delta = get_player_delta(player, bezier_curve[last_bizier_point_index]);
        }
        
        player.x += delta.x;
        player.y += delta.y;
        p.setPosition(player.x, player.y);
        cout << player.x <<  " "  << player.y << endl;
        
        window.draw(p);

        window.display();
    }

    return 0;
}
