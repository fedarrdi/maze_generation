#include <SFML/Graphics.hpp>
#include<iostream>
#include <vector>

using namespace std;

const int WINDOW_SIZE_X = 800, WINDOW_SIZE_Y = 800;
const int CELLS_X = 100, CELLS_Y = 100;
const int DRAW_SIZE_X = (WINDOW_SIZE_X / 1) / CELLS_X;
const int DRAW_SIZE_Y = (WINDOW_SIZE_Y / 1) / CELLS_Y;

                         //l, r, u, d
struct cell_neighbours { int ind[4]; };

struct cell_neighbours get_neighbours(int index)
{
    cell_neighbours out;
    
    out.ind[0] = (!(index % CELLS_X)) ? -1 : index - 1;
    out.ind[1] = (!((index + 1) % CELLS_X)) ? -1 : index + 1;
    out.ind[2] = (index - CELLS_X < 0) ? -1 : index - CELLS_X; 
    out.ind[3] = (index + CELLS_X >= CELLS_X * CELLS_Y) ? -1 : index + CELLS_X;
            
    return out;
}

vector<cell_neighbours> generate_graph()
{
    vector<cell_neighbours> out;
    for(int curr_cell_index = 0;curr_cell_index < CELLS_X * CELLS_Y;curr_cell_index++)
    {
        cell_neighbours curr_cell_neighbours = get_neighbours(curr_cell_index);   
        out.push_back(curr_cell_neighbours);
    
    }
    return out;
}

void dfs(int curr, bool *vis,const vector<cell_neighbours> &p, cell_neighbours *out)
{
    vis[curr] = 1;

    for(int i = 0;i < 4;i++)
    {
        int next = p[curr].ind[i];
        if(next != -1 && !vis[next])
        { 
            out[curr].ind[i] = next;
            
            int index;
            if(i == 0) index = 1;
            if(i == 1) index = 0;
            if(i == 2) index = 3;
            if(i == 3) index = 2;
        
            out[next].ind[index] = curr;

            dfs(next, vis, p, out);
        }
    }

    return;
}

cell_neighbours *vis_graph(const vector<cell_neighbours> &p)
{
    bool vis[CELLS_Y * CELLS_Y];
    for(int i = 0;i < CELLS_X * CELLS_Y;vis[i++] = 0);

    cell_neighbours *out =  new cell_neighbours[CELLS_X * CELLS_Y];

    for(int i = 0;i < CELLS_X * CELLS_Y;i++)
        for(int j = 0;j < 4;j++)
            out[i].ind[j] = -1;

    dfs(0, vis, p, out); 

    return out;
}


vector<sf::RectangleShape> draw_maze(cell_neighbours * maze)
{
    int index = 0, x1 = 0, y1 = 0, y2 = DRAW_SIZE_Y, x2 = DRAW_SIZE_X;
    int t = 0;
    
    vector< sf::RectangleShape > out;

    for(int i = 0;i < CELLS_X * CELLS_Y;i++)
    {
        cell_neighbours curr_n = maze[index];
            
        if(!(t % CELLS_X))
        { 
            x1 = 0;
            x2 = DRAW_SIZE_X;
            y1 += DRAW_SIZE_Y;
            y2 += DRAW_SIZE_Y;
        }

        for(int j = 0;j < 4;j++)
        {
            if(curr_n.ind[j] == -1)
            {
                if (j == 0)
                {
                    sf::RectangleShape curr;
                    curr.setSize(sf::Vector2f(1, DRAW_SIZE_Y));
                    curr.setPosition(x1, y1);
                    out.push_back(curr);
                    cout << "a" << endl;
                }
                
                if(j == 1)
                {
                    sf::RectangleShape curr;
                    curr.setSize(sf::Vector2f(1, DRAW_SIZE_Y));
                    curr.setPosition(x2, y1);
                    out.push_back(curr);
                    cout << "b" << endl;
                }
                
                if(j == 2)
                {
                    sf::RectangleShape curr;
                    curr.setSize(sf::Vector2f(DRAW_SIZE_X, 1));
                    curr.setPosition(x1, y1);
                    out.push_back(curr);
                    cout << "c" << endl;
                }
                
                if(j == 3)
                {
                    sf::RectangleShape curr;
                    curr.setSize(sf::Vector2f(DRAW_SIZE_X, 1));
                    curr.setPosition(x1, y2);
                    out.push_back(curr);
                    cout << "d" << endl;
                }
            }
        }
        x1 += DRAW_SIZE_X;
        x2 += DRAW_SIZE_X;
        
        t++;
        index++;
    }


    return out;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_Y, WINDOW_SIZE_X), "SFML works!");
    
    vector<cell_neighbours> g = generate_graph();
    
    for(int i = 0;i < CELLS_X * CELLS_Y;i++)
    {
        cout << "i: " << i << endl;
        for(int j = 0;j < 4;j++)
            cout << g[i].ind[j] << " ";
        cout << endl;
    }

    cout << "=============================" << endl;

    cell_neighbours *maze = vis_graph(g);
    for(int i = 0;i < CELLS_X * CELLS_Y;i++)
    {
        cout <<  "i: " << i << endl;
        for(int j = 0;j < 4;j++)
            cout << maze[i].ind[j] << " ";
        cout << endl;
    }

    
    vector<sf::RectangleShape> lines = draw_maze(maze);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
         
       
        window.clear();
        for(int i = 0;i < lines.size();i++)
            window.draw(lines[i]);
        window.display();
    }

    return 0;
}
