#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>

using namespace std;

const int WIDTH = 800, HEIGHT = 800;

struct Point {double x, y;};

class Boid
{

private:
    Point pos_cords, bizier_curve_point_b, destination, delta;
    vector<Point> bizier_curve;
    int last_bizier_point_index;
    sf::CircleShape draw_point;

public:
    
    sf::CircleShape get_draw_point() const { return draw_point; }

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

    void get_player_delta()
    {
        this->delta.x = (bizier_curve[last_bizier_point_index].x - pos_cords.x) / 100;
        this->delta.y = (bizier_curve[last_bizier_point_index].y - pos_cords.y) / 100; 
    }

    Point generate_random_point(double min_dist)
    {
        double d = 0;
        Point e;

        while(d < min_dist)
        {
            e.x = rand() % WIDTH;
            e.y = rand() % HEIGHT;
            d = dist(e, pos_cords);
        }
        return e;
    }

    Point generate_bizier_curve_point(double min_dist)
    {
        double d1, d2;
        Point b;
        do
        {
            b.x = rand() % WIDTH;
            b.y = rand() % HEIGHT;
            d1 = dist(pos_cords, b);
            d2 = dist(destination, b);
        }while(d1 < min_dist || d2 < min_dist);
    
        return b;
    }

    vector<Point> generate_bizier_curve(double t_step)
    {
        vector<Point> bezier_curve;
        for(double t = 0;t <= 1;t += t_step)
        {
            Point curr_point1 = move_point(pos_cords, bizier_curve_point_b, t);
            Point curr_point2 = move_point(bizier_curve_point_b, destination, t);
            Point bezier_point = move_point(curr_point1, curr_point2, t);
            bezier_curve.push_back(bezier_point);
        }

        return bezier_curve;
    }
    
    Boid()
    {   
        this->pos_cords.x = rand() % WIDTH;
        this->pos_cords.y = rand() % HEIGHT;
        this->last_bizier_point_index = 0;
        draw_point.setRadius(4);
        draw_point.setPosition(pos_cords.x, pos_cords.y);
    }

    void move()
    {   

        if(last_bizier_point_index == bizier_curve.size())
        {   
            this->bizier_curve.clear();
            this->destination = generate_random_point(300);
            this->bizier_curve_point_b = generate_bizier_curve_point(150);
            this->last_bizier_point_index = 0;
            this->bizier_curve = generate_bizier_curve(0.01);
            get_player_delta();
        }

        if(dist(pos_cords,  bizier_curve[last_bizier_point_index]) < 0.05)
        {
            last_bizier_point_index++;
            get_player_delta();
        }
        this->pos_cords.x += delta.x;
        this->pos_cords.y += delta.y;
        cout << pos_cords.x <<  " " << pos_cords.y << endl;
        draw_point.setPosition(pos_cords.x, pos_cords.y);
    }
    
};

int main()
{
    srand(static_cast<unsigned int> (time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
    
    Boid boid[20];
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
       
        window.clear();
        for(int i = 0;i < 20;i++)
        {
            boid[i].move();
            window.draw(boid[i].get_draw_point());
        }
        window.display();
    }

    return 0;
}

