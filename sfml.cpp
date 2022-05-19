#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

const int WINDOW_SIZE_X = 1000, WINDOW_SIZE_Y = 1000;
const int CELLS_X = 100, CELLS_Y = 100;
const int DRAW_SIZE_X = (WINDOW_SIZE_X / 1.1) / CELLS_X;
const int DRAW_SIZE_Y = (WINDOW_SIZE_Y / 1.1) / CELLS_Y;

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

void randomize(vector<pair<int, int>> &p)
{
    for(int i = 0;i < 10;i++)
    {
        int f_i = rand() % p.size();
        int s_i = rand() % p.size();
        
        auto tmp = p[f_i];
        p[f_i] = p[s_i];
        p[s_i] = tmp;
    }
}


void dfs(int curr, bool *vis,const vector<cell_neighbours> &p, cell_neighbours *out)
{
    vis[curr] = true;
    
    vector<pair<int, int>> options;    
    
    for(int i = 0;i < 4;i++)
    {
        int next = p[curr].ind[i];
        options.push_back({i, next});
    }   

    randomize(options);

    for(auto next : options)
    {       
        if(next.second != -1 && !vis[next.second])
        {
            int index;
            if(next.first == 0) index = 1;
            if(next.first == 1) index = 0;
            if(next.first == 2) index = 3;
            if(next.first == 3) index = 2;
        
            out[curr].ind[next.first] = next.second;
            out[next.second].ind[index] = curr;     
            
            dfs(next.second, vis, p, out); 
        }
    }


    return;
}

cell_neighbours *vis_graph(const vector<cell_neighbours> &p)
{
    bool vis[CELLS_Y * CELLS_Y];
    for(int i = 0;i < CELLS_X * CELLS_Y;vis[i++] = 0);

    cell_neighbours *out = new cell_neighbours[CELLS_X * CELLS_Y];

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
                }
                
                if(j == 1)
                {
                    sf::RectangleShape curr;
                    curr.setSize(sf::Vector2f(1, DRAW_SIZE_Y));
                    curr.setPosition(x2, y1);
                    out.push_back(curr);
                }
                
                if(j == 2)
                {
                    sf::RectangleShape curr;
                    curr.setSize(sf::Vector2f(DRAW_SIZE_X, 1));
                    curr.setPosition(x1, y1);
                    out.push_back(curr);
                }
                
                if(j == 3)
                {
                    sf::RectangleShape curr;
                    curr.setSize(sf::Vector2f(DRAW_SIZE_X, 1));
                    curr.setPosition(x1, y2);
                    out.push_back(curr);
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

void form_cycles_in_maze(cell_neighbours *out)
{
    int cycle_count = CELLS_X * CELLS_Y / 10;

    for(int i = 0;i < cycle_count;i++)
    {
        int curr_node = rand() % (CELLS_X * CELLS_Y);
        int direction = rand() % 4;
        int neighbour_node = -1;
        
        if(direction == 0 && curr_node % CELLS_X)    
            neighbour_node = curr_node - 1;
    
        if(direction == 1 && (curr_node + 1) % CELLS_X)
            neighbour_node = curr_node + 1;
    
        if(direction == 2 && curr_node - CELLS_X > 0)
            neighbour_node = curr_node - CELLS_X;

        if(direction == 3 && curr_node + CELLS_X < CELLS_X * CELLS_Y)
            neighbour_node = curr_node + CELLS_X;
        
        if(neighbour_node != -1)
        {
            out[curr_node].ind[direction] = neighbour_node;
            
            int index;
            if(direction == 0) index = 1;
            if(direction == 1) index = 0;
            if(direction == 2) index = 3;
            if(direction == 3) index = 2;
            out[neighbour_node].ind[index] = curr_node;
        }
        else
            i--;
    }
}

void generate_room(cell_neighbours *out, int start_node, int depth)
{
    if(!depth) return;
    out[start_node].ind[0] = start_node - 1;
    out[start_node - 1].ind[1] = start_node;
    out[start_node].ind[1] = start_node + 1;
    out[start_node + 1].ind[0] = start_node;
    out[start_node].ind[2] = start_node - CELLS_X;
    out[start_node - CELLS_X].ind[3] = start_node;
    out[start_node].ind[3] = start_node + CELLS_X;
    out[start_node + CELLS_X].ind[2] = start_node;   

    int f_d = rand() % 4, s_d, t = rand() % 2;
    int f_node, s_node;
    
    if(f_d == 0 || f_d == 1)
        s_d = t ? 2 : 3;

    else if(f_d == 2 || f_d == 3)
        s_d = t ? 0 : 1;
    
    if(f_d == 0) f_node = start_node - 1;
    
    if(f_d == 1) f_node = start_node + 1;

    if(f_d == 2) f_node = start_node - CELLS_X;

    if(f_d == 3) f_node = start_node + CELLS_X;

    if(s_d == 0) s_node = start_node - 1;
    
    if(s_d == 1) s_node = start_node + 1;

    if(s_d == 2) s_node = start_node - CELLS_X;

    if(s_d == 3) s_node = start_node + CELLS_X;

    generate_room(out, f_node, depth - 1);
    generate_room(out, s_node, depth - 1);
}

void make_rooms(cell_neighbours *out)
{
    int room_size = 4;
    int rooms_count = 5;
    
    for(int i = 0;i < rooms_count;i++)
    {
        int room_start_node = rand() % (CELLS_X * CELLS_Y);
        
        int y = room_start_node / CELLS_X;
        int x = room_start_node - CELLS_X * y;
        

        if((y - room_size >= 0 && y + room_size < CELLS_X * CELLS_Y) && (x - room_size >= 0 && x + room_size < CELLS_X))
           generate_room(out, room_start_node, room_size);
        else
            i--;
    }
    
}

vector<sf::RectangleShape> draw_path(vector<int> path)
{

    vector<sf::RectangleShape> out;
    cout << path.size() << endl;

    for(int curr_node : path)
    {
      
        int x = 0, y = 0, tmp = curr_node;

        while(tmp - CELLS_X >= 0)
        {
            y++;
            tmp -= CELLS_X;
        }
        y++; 
        x = tmp;
        
        sf::RectangleShape curr_sq;
        curr_sq.setFillColor(sf::Color(100, 250, 50));
        curr_sq.setSize(sf::Vector2f(DRAW_SIZE_X / 2, DRAW_SIZE_Y / 2));
        curr_sq.setPosition(x*DRAW_SIZE_X, y*DRAW_SIZE_Y);
        
        out.push_back(curr_sq);
    }   

    return out;
}

void dijickstra(int curr, int *dist, const cell_neighbours *g, int *parent_of_node)
{
    for(int i = 0;i < 4;i++)
    {
        int next = g[curr].ind[i], curr_dist = dist[curr] + 1;
        
        if(next != -1 && dist[next] > curr_dist)
        {
            dist[next] = curr_dist;
            parent_of_node[next] = curr;
            dijickstra(next, dist, g, parent_of_node);
        }   
    }        
}

void print_path(int index, const int *parent, vector<int> &path)
{   
    if(index == -1)
        return;
    
    print_path(parent[index], parent, path);
    path.push_back(index);
    cout << index << endl;
}

vector<int> find_path_from_to(const cell_neighbours *g)
{
    int start_node = 0, end_node = CELLS_X * CELLS_Y - 1,  dist[CELLS_X * CELLS_Y], parent_of_node[CELLS_X * CELLS_Y];
    vector<int> path;    
    for(int i = 0;i < CELLS_X * CELLS_Y;parent_of_node[i] = -1, dist[i++] = 1e5);
    dist[start_node] = 0;
    dijickstra(start_node, dist, g, parent_of_node);
    print_path(end_node, parent_of_node, path);
    return path;
}


int main()
{
    srand(static_cast<unsigned int> (time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_Y, WINDOW_SIZE_X), "SFML works!");
    
    vector<cell_neighbours> g = generate_graph();
    
    cell_neighbours *maze = vis_graph(g);
    
    form_cycles_in_maze(maze);

    make_rooms(maze);

    vector<sf::RectangleShape> lines = draw_maze(maze);
    
    vector<int> path = find_path_from_to(maze);
    vector<sf::RectangleShape> path_cords =draw_path(path);
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

        for(int i = 0;i < path_cords.size();i++)
            window.draw(path_cords[i]);
            
        window.display();
    }

    return 0;
}
