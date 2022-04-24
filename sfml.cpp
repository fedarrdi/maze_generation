#include <SFML/Graphics.hpp>
#include<iostream>
#include <vector>

const int WINDOW_SIZE_X = 800, WINDOW_SIZE_Y = 800;
const int SIZE_X = 3, SIZE_Y = 3;

const int VERTICAL_LINE_COUNT = (SIZE_X + 1) * SIZE_Y;
const int HORIZONTAL_LINE_COUNT = (SIZE_Y + 1) * SIZE_X;

const int DRAW_SIZE_X = WINDOW_SIZE_X / SIZE_X;
const int DRAW_SIZE_Y = WINDOW_SIZE_Y / SIZE_Y;

sf::RectangleShape horizontal_line[HORIZONTAL_LINE_COUNT], vertical_line[VERTICAL_LINE_COUNT];

using namespace std;

void build_maze_walls()
{
    for(int i = 0, y = 0, x = 0, j = 0;i < HORIZONTAL_LINE_COUNT;i++, j++)
    {   
        if(j == SIZE_X)
        {
            j = 0;
            y += DRAW_SIZE_Y;
            x = 0;
        }
        else
            x += DRAW_SIZE_X;
            
        horizontal_line[i].setSize(sf::Vector2f(DRAW_SIZE_X - 1, 1));
        horizontal_line[i].setPosition(x, y);
    }

    for(int i = 0, y = 0, x = 0, j = 0;i < VERTICAL_LINE_COUNT;i++, j++)
    {
        if(j == SIZE_Y)
        {
            j = 0;
            x += DRAW_SIZE_X;
            y = 0;
        }
        else
            y += DRAW_SIZE_Y;

        vertical_line[i].setSize(sf::Vector2f(1, DRAW_SIZE_Y - 1));
        vertical_line[i].setPosition(x, y);
    }
}


vector<vector<int>> creat_graph()
{
    vector<vector<int>> e;
    e.resize(SIZE_X * SIZE_Y);
    
    for(int y = 0;y < SIZE_Y;y++)
    {
        for(int x = 0;x < SIZE_X;x++)
        {
            int curr_node = SIZE_X * y + x;
            int f = x - 1;
            int s = x + 1;
            int q = curr_node - SIZE_Y;
            int p = curr_node + SIZE_Y;

            if(f >= 0)
                e[curr_node].push_back(curr_node - 1);
            
            if(s < SIZE_X)
                e[curr_node].push_back(curr_node + 1);

            if(q >= 0)
                e[curr_node].push_back(q);

            if(p < SIZE_X * SIZE_Y)
                e[curr_node].push_back(p);       
        }
    }
    return e;
}

void print_vector(vector<vector<int>> e)
{
    for(int i = 0;i < e.size();i++)
    {   
        cout << "parent: " << i << endl;
        cout << "children -> ";
        for(int j = 0;j < e[i].size();j++)
        {
            cout << e[i][j] << " ";
        }
        cout << endl << endl;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_Y, WINDOW_SIZE_X), "SFML works!");
    build_maze_walls();

    vector<vector<int>> e = creat_graph();
    print_vector(e);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
         
       
        window.clear();
        
        for(int i = 0;i < VERTICAL_LINE_COUNT;i++)    
            window.draw(vertical_line[i]);

        for(int i = 0;i < HORIZONTAL_LINE_COUNT;i++)
            window.draw(horizontal_line[i]);

        window.display();
    }

    return 0;
}


