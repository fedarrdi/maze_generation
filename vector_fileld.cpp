#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <cmath>

using namespace std;


class Point
{

private:
    double x, y;

public:

    Point() {}

    Point(double x, double y) : x(x), y(y) { }

    Point (const Point &other)
    {
        x = other.x;
        y = other.y;
    }

    Point& operator = (const Point &other)
    {
        if(this != &other)
        {
            x = other.x;
            y = other.y;
        }

        return *this;
    }

    Point operator - (const Point &other) const
    {
        Point res(x - other.get_x(), y - other.get_y());
        return res;
    }

    Point operator / (double dev) const
    {
        Point res(x / dev, y / dev);
        return res;
    }

    Point& operator += (const Point &other)
    {
        x += other.get_x();
        y += other.get_y();
        return *this;
    }

    void set_x(double x) { this->x = x; }

    void set_y(double y) { this->y = y; }

    double get_x() const { return x; }

    double get_y() const { return y; }

    void print() const
    {
        cout << "x = " << x << " " << "y = " << y << endl;
    }
};

class Vector
{

private:
    Point s, e;

public:
    Vector() { }

    Vector(Point s, Point e) : s(s), e(e) { }

    Vector(const Vector &other)
    {
        s = other.s;
        e = other.e;
    }

    Point get_s() { return s; }

    Point get_e() { return e; }

    void set_s(const Point &s) { this->s = s; }

    void set_e(const Point &e) { this->e = e; }

    double get_angle(const Vector& other)
    {
        Point w(other.e.get_x() - s.get_x(), other.e.get_y() - s.get_y())
        ,v(e.get_x() - s.get_x(), e.get_y() - s.get_y());

        return atan2(w.get_y()*v.get_x() - w.get_x()*v.get_y(),
                     w.get_x()*v.get_x() + w.get_y()*v.get_y());
    }

    Vector& operator = (const Vector &other)
    {
        if(this != &other)
        {
            s = other.s;
            e = other.e;
        }
        return *this;
    }

    Vector operator + (const Vector &other)
    {
        Vector res;
        res.s = s;
        res.e.set_x(e.get_x() + other.e.get_x() - s.get_x());
        res.e.set_y(e.get_y() + other.e.get_y() - s.get_y());
        return res;
    }

    void print()
    {
        s.print();
        e.print();
    }
};

class Particle
{

private:
    Point position;
    Vector direction;

public:

    Particle() {}

    Particle(const Point & position,const Vector & direction) : position(position), direction(direction) {}

    void set_position(const Point &position)
    {
        this->position = position;
    }

    void set_direction(const Vector &direction)
    {
        this->direction = direction;
    }

    Point get_position() const { return position; }

    Vector get_direction() const { return direction; }

    void move()
    {
        Point delta = direction.get_e() - direction.get_s();
        delta = delta / 100;
        position += delta;
    }

    sf::RectangleShape get_draw_obj()
    {
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(5, 5));
        rectangle.setPosition(position.get_x(), position.get_y());
        rectangle.setFillColor(sf::Color::Red);
        return rectangle;
    }

    void print_particle()
    {
        cout << "Position: ";
        position.print();
        cout << "Direction ";
        direction.print();
    }

};

class Vector_field
{
private:
    unsigned field_width, field_height;
    unsigned cell_width, cell_height;
    Vector **field_directions;

public:
    Vector_field() {}

    Vector_field(unsigned field_width, unsigned field_height, unsigned cell_width, unsigned cell_height)
    {
        this->field_width = field_width;
        this->field_height = field_height;
        this->cell_width = cell_width;
        this->cell_height = cell_height;

        field_directions = new Vector* [field_width];
        for(int i = 0;i < field_width;i++)
            field_directions[i] = new Vector [field_height];
    }

    void fill_vector_field()
    {
        for(int y = 0;y < field_height;y++)
            for(int x = 0;x < field_width;x++)
            {
                int x_  = x*x - 9*x, y_ = y*y - 9*y;

                field_directions[y][x].set_s(Point(0, 0));
                field_directions[y][x].set_e(Point(x_, y_));
            }
    }

    vector<sf::RectangleShape> generate_grid()
    {
        vector<sf::RectangleShape> out;
        sf::RectangleShape rectangle;
        
        for(int y = 0;y < field_height;y++)
        {
            for(int x = 0;x < field_width;x++)
            {
                int draw_x = x * cell_width, draw_y = y * cell_height;
                
                rectangle.setPosition(draw_x, draw_y);
                rectangle.setSize(sf::Vector2f(cell_width - 1, cell_height - 1));
                out.push_back(rectangle);
            }
        }

        return out;
    }

    vector<sf::Vertex> generate_vectors()
    {
        vector<sf::Vertex> out;
        
        for(int i = 0;i < field_height;i++)
        {
            for(int j = 0;j < field_width;j++)
            {
                Point offset = field_directions[i][j].get_e();///need to scale down vectos
                out.push_back(sf::Vertex(sf::Vector2f((j + 0.5) * cell_width, (i + 0.5) * cell_height), sf::Color::Red));
                out.push_back(sf::Vertex(sf::Vector2f((j + 0.5) * cell_width + offset.get_x(), (i + 0.5) * cell_height + offset.get_y()), sf::Color::Red));               
            }
        }

        return out;
    }
    
    int get_collision_index(Point particle_position)
    {
        int x_index = particle_position.get_x() / cell_width - 0.5;
        int y_index = particle_position.get_y() / cell_height - 0.5;
        
        return y_index * field_width + x_index;
    }

    void print_vector_field()
    {
        int index = 0;
        for(int i = 0;i < field_height;i++)
        {
            for(int j = 0;j < field_width;j++)
            {
                cout << index++ << endl;
                field_directions[i][j].print();
                cout << endl;
           }    
        }
    }
};


int main()
{
    const int WIDTH = 800, HEIGHT = 800;

    srand(static_cast<unsigned int> (time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
   
    Point a(200, 200), b(0, 0), c(10, 10);
    Vector v(b, c);

    Vector_field field(30, 30, WIDTH / 30, HEIGHT / 30);
    Particle particle(a, v);
    
    field.fill_vector_field();
    
    vector<sf::RectangleShape> f = field.generate_grid(); 
    vector<sf::Vertex> vs = field.generate_vectors();

    int last_index = -1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //particle.move(); 
        
        window.clear();
        
        int index = field.get_collision_index(particle.get_position());
        f[index].setFillColor(sf::Color::Blue);
    
        if(-1 == last_index)
            last_index = index;

        if(last_index != index)
        {
            f[last_index].setFillColor(sf::Color::White);
            last_index = index;
        }

        for(auto curr : f)
            window.draw(curr);

        for(int i = 0;i < vs.size(); i+= 2)
        {
            sf::Vertex line[2] =
            {
                vs[i],
                vs[i + 1]
            };

            window.draw(line, 2, sf::Lines);
        }

        //window.draw(particle.get_draw_obj());
        
        window.display();
    }

    return 0;
}
