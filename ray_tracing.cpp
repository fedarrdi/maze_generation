#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

const int RAYS_COUNT = 10;
const int RAYS_LENGTH = 400;
const int WIDTH = 800;
const int HEIGHT = 800;
const int WALLS_COUNT = 20;

struct rectangle 
{
    double x, y;
    int w, h;
};

struct point { double x, y; };
struct line { point a, b; };

vector<sf::RectangleShape> generate_obsticles(line *obs)
{   
    vector<sf::RectangleShape> out;

    for(int i = 0;i < WALLS_COUNT;i++)
    {
        sf::RectangleShape curr_line;
        int x = rand() % WIDTH, y = rand() % HEIGHT;

        curr_line.setPosition(sf::Vector2f(x, y));

        obs[i].a.x = x;
        obs[i].a.y = y;
        
        int size = rand() % 100;

        if(rand() % 2 == 1) 
        {
            obs[i].b.x = x + size;
            obs[i].b.y = y;
            curr_line.setSize(sf::Vector2f(size, 1));
        }
        else 
        {
            obs[i].b.x = x;
            obs[i].b.y = y + size;
            curr_line.setSize(sf::Vector2f(1, size));
        }

        out.push_back(curr_line);    
    }
    return out;
}

vector<sf::RectangleShape> generate_rays(rectangle player_cords, line *rays_cords)
{
    vector<sf::RectangleShape> out;
    double angle = 0, offset = 360 / RAYS_COUNT;
   
    for(int i = 0;i < RAYS_COUNT;i++)
    {
        sf::RectangleShape curr_line;

        curr_line.setPosition(sf::Vector2f(player_cords.x + player_cords.w / 2, player_cords.y + player_cords.h / 2));
        curr_line.setSize(sf::Vector2f(1, RAYS_LENGTH));
        curr_line.rotate(angle);
        out.push_back(curr_line);


        rays_cords[i].a.x = player_cords.x + player_cords.w / 2;
        rays_cords[i].a.y = player_cords.y + player_cords.h / 2;

        rays_cords[i].b.x = player_cords.x + player_cords.w / 2 + RAYS_LENGTH * cos(angle);
        rays_cords[i].b.y = player_cords.y + player_cords.h / 2 + RAYS_LENGTH * sin(angle);
        
        angle += offset;
    }
    return out;
}

void move_player(rectangle *player_cords, sf::RectangleShape *player, vector<sf::RectangleShape> &rays, line *line_cords)
{
    double speed = 0.1;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) player_cords->x -= speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player_cords->x += speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player_cords->y -= speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player_cords->y += speed;

    player->setPosition(player_cords->x, player_cords->y);    

    for(int i = 0;i < rays.size();i++)
    {
        rays[i].setPosition(sf::Vector2f(player_cords->x + player_cords->w / 2, player_cords->y + player_cords->h / 2));     
        point delta {player_cords->x - line_cords[i].a.x, player_cords->y - line_cords[i].a.y};    

        line_cords[i].a.x += delta.x;
        line_cords[i].a.y += delta.y;
        line_cords[i].b.x += delta.x;
        line_cords[i].b.y += delta.y;
    }
}

bool findIntersection(line a, line b, point* result)
{
    float x1 = a.a.x, y1 = a.a.y, x2 = a.b.x, y2 = a.b.y, x3 = b.a.x, y3 = b.a.y, x4 = b.b.x, y4 = b.b.y;

    float a1 = y2 - y1;
    float b1 = x1 - x2;
    float c1 = a1 * x1 + b1 * y1;

    float a2 = y4 - y3;
    float b2 = x3 - x4;
    float c2 = a2 * x3 + b2 * y3;

    float d = a1 * b2 - a2 * b1;

    if (!d) return false;
    float x = (b2 * c1 - b1 * c2) / d;
    float y = (a1 * c2 - a2 * c1) / d;

    bool online1 =
            ((fmin(x1, x2) < x || fmin(x1, x2) == x) &&
             (fmax(x1, x2) > x || fmax(x1, x2) == x) &&
             (fmin(y1, y2) < y || fmin(y1, y2) == y) &&
             (fmax(y1, y2) > y || fmax(y1, y2) == y));
    bool online2 =
            ((fmin(x3, x4) < x || fmin(x3, x4) == x) &&
             (fmax(x3, x4) > x || fmax(x3, x4) == x) &&
             (fmin(y3, y4) < y || fmin(y3, y4) == y) &&
             (fmax(y3, y4) > y || fmax(y3, y4) == y));
    
    if (online1 && online2)
    {
        result->x = x, result->y = y;
        return true;
    }

    return false;
}

void find_rays_length(line *line_cords, line *obs_cords, vector<sf::RectangleShape> &rays)
{
    double lengths[RAYS_COUNT];
    
    for(int i = 0;i < rays.size();lengths[i++] = RAYS_LENGTH * RAYS_LENGTH);

    for(int i = 0;i < RAYS_COUNT;i++)
        for(int j = 0;j < WALLS_COUNT ;j++)
        {
            point intersection;
            if(findIntersection(line_cords[i], obs_cords[j], &intersection))
            {
                double rx = line_cords[i].a.x - intersection.x;
                double ry = line_cords[i].a.y - intersection.y;
                double dist = rx*rx + ry*ry;
                cout << dist << endl;      
    
                if(lengths[i] > dist)
                    lengths[i] = dist;
            }
        }

    for(int i = 0;i < RAYS_COUNT;i++)
    {
        //cout << lengths[i] << endl;
        //rays[i].setSize(sf::Vector2f(1, sqrt(lengths[i])));   
    }
}

int main()
{

    srand (static_cast <unsigned> (time(0)));
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
    
    rectangle player_cords = {10, 10, 30, 30}; 
    sf::RectangleShape player;
    player.setSize(sf::Vector2f(player_cords.w, player_cords.h));
    
    line rays_cords[RAYS_COUNT];
    line obs_cords[WALLS_COUNT];
    

    vector<sf::RectangleShape> rays = generate_rays(player_cords, rays_cords);
    vector<sf::RectangleShape> obs = generate_obsticles(obs_cords);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        move_player(&player_cords, &player, rays, rays_cords);
        find_rays_length(rays_cords, obs_cords, obs); 
        
        window.clear();
        
        window.draw(player);
        
        for(int i = 0;i < WALLS_COUNT;i++)
            window.draw(obs[i]);

        for(int i = 0;i < RAYS_COUNT;i++)
            window.draw(rays[i]);
        
        window.display();
    }

    return 0;
}
