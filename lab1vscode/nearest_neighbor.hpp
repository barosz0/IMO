#include "parser.hpp"
#include <cstdlib>
#include <list>

class Nearest_neighbor
{
private:
    vector<vector<int>> matrix;
    vector<pair<double, double>> coords; 
    
    void choose_random_starting(int &a,int &b);
    int next_pick(vector<bool> taken, int last_node);
    int get_element(list<int> _list, int index);
    
public:
    
    list<int> A_cycle;
    list<int> B_cycle;
    int A_length;
    int B_length;

    void run(int a, int b);
    vector<pair<double, double>> get_cycle_coords(list<int> cycle);
    Nearest_neighbor(string file);
    ~Nearest_neighbor();
};

vector<pair<double, double>> Nearest_neighbor::get_cycle_coords(list<int> cycle)
{
    vector<pair<double, double>> cycle_coords;
    list<int>::iterator i;
    
    for (i = cycle.begin(); i != cycle.end(); ++i)
        cycle_coords.push_back(coords[*i]);

    return (cycle_coords);
}

int Nearest_neighbor::get_element(list<int> _list, int index)
{
    list<int>::iterator it = _list.begin();
    advance(it,index);
    return *it;
}

int Nearest_neighbor::next_pick(vector<bool> taken, int last_node)
{
    int min_cost = 10000000;
    int cost;
    int selected = -1;
    for(int j = 0; j < matrix[last_node].size(); j++)
    {
        if(!taken[j]){

             cost = matrix[last_node][j];
                
                if(cost < min_cost)
                {
                    min_cost = cost;
                    selected = j;
                }

        }
    }
    return selected;
}

void Nearest_neighbor::choose_random_starting(int &a, int &b)
{
    srand((unsigned) time(NULL));
    a = rand() % coords.size();

    b = 0;
    for(int i = 1; i < coords.size(); i++)
    {
        if(matrix[a][b] < matrix[a][i])
        {
            b = i;
        }
    }
}

void Nearest_neighbor::run(int a, int b)
{

    cout <<"Zaczynam od a:" << a << " b:" << b << endl;
    list<int> A{a};
    A_length = 0;
    list<int> B{b};
    B_length = 0;
    
    vector<bool> taken(coords.size());
    taken[b] = taken[a] = true;

    int a_last, b_last;
    int a_cost, b_cost; 

    while(A.size() + B.size() < coords.size())
    {
        a = next_pick(taken,a_last);
        a_cost = matrix[a_last][a];
        taken[a] = true;

        b = next_pick(taken,b_last);
        b_cost = matrix[b_last][b];
        taken[b] = true;

        A_length += a_cost;
        B_length += b_cost;

       
        A.push_back(a);
        B.push_back(b);

        a_last = a;
        b_last =b;
        

    }

    A_cycle = A;
    B_cycle = B;
}

Nearest_neighbor::Nearest_neighbor(string file)
{
    parser p(file);

    matrix = p.matrix;
    coords = p.coords;

    int a,b;
    A_length = 0;
    B_length = 0;

    choose_random_starting(a,b);
    run(a,b);
}

Nearest_neighbor::~Nearest_neighbor()
{
}

