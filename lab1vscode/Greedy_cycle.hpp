#include "parser.hpp"
#include <cstdlib>
#include <list>

class Greedy_cycle
{
private:
    vector<vector<int>> matrix;
    vector<pair<double, double>> coords; 
    
    void choose_random_starting(int &a,int &b);
    int next_pick(list<int> cycle, vector<bool> taken, pair<int,int> &selected_pair);
    int get_element(list<int> _list, int index);
    
public:
    list<int> A_cycle;
    list<int> B_cycle;
    int A_length;
    int B_length;

    void run(int a, int b);
    vector<pair<double, double>> get_cycle_coords(list<int> cycle);
    Greedy_cycle(string file);
    ~Greedy_cycle();
};

vector<pair<double, double>> Greedy_cycle::get_cycle_coords(list<int> cycle)
{
    vector<pair<double, double>> cycle_coords;
    list<int>::iterator i;
    
    for (i = cycle.begin(); i != cycle.end(); ++i)
        cycle_coords.push_back(coords[*i]);

    return (cycle_coords);
}

int Greedy_cycle::get_element(list<int> _list, int index)
{
    list<int>::iterator it = _list.begin();
    advance(it,index);
    return *it;
}

int Greedy_cycle::next_pick(list<int> cycle, vector<bool> taken, pair<int,int> &selected_pair)
{
    pair<int,int> min_p;

    int selected = -1;
    int min_cost = -1;

    int new_cost;
    for(int j = 0; j < cycle.size(); j++)
    {
        pair<int,int> new_p;

        new_p.first = get_element(cycle, j); //cycle[j];


        if(j+1 == cycle.size())
            new_p.second = get_element(cycle, 0);//cycle[0];
        else
            new_p.second = get_element(cycle, j+1);//cycle[j+1]
        for(int i = 0; i < coords.size(); i++)
        {
            if(!taken[i])
            {
                if(selected == -1)
                {
                    selected = i;
                    min_cost = matrix[new_p.first][i] + matrix[new_p.second][i] - matrix[new_p.first][new_p.second]; 
                    min_p = new_p;
                    continue;
                }

                new_cost = matrix[new_p.first][i] + matrix[new_p.second][i] - matrix[new_p.first][new_p.second];
                
                if(new_cost < min_cost)
                {
                    min_p = new_p;
                    min_cost = new_cost;
                    selected = i;
                }
            }    
        }
    }
    selected_pair = min_p;
    return selected;
}

void Greedy_cycle::choose_random_starting(int &a, int &b)
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

void Greedy_cycle::run(int a, int b)
{

    cout <<"Zaczynam od a:" << a << " b:" << b << endl;
    list<int> A{a};
    A_length = 0;
    list<int> B{b};
    B_length = 0;
    
    vector<bool> taken(coords.size());
    taken[b] = taken[a] = true;

    pair<int,int> a_place, b_place;
    int a_cost, b_cost; 

    while(A.size() + B.size() < coords.size())
    {
        a = next_pick(A,taken,a_place);
        a_cost = matrix[a_place.first][a] + matrix[a_place.second][a] - matrix[a_place.first][a_place.second];

        b = next_pick(B,taken,b_place);
        b_cost = matrix[b_place.first][b] + matrix[b_place.second][b] - matrix[b_place.first][b_place.second];

        taken[a] = true;
        taken[b] = true;

        if(a==b)
        {
            if(a_cost<b_cost)
            {
                b = next_pick(B,taken,b_place);
                b_cost = matrix[b_place.first][b] + matrix[b_place.second][b] - matrix[b_place.first][b_place.second];
                taken[b] = true;
            }
            else
            {
                a = next_pick(A,taken,a_place);
                a_cost = matrix[a_place.first][a] + matrix[a_place.second][a] - matrix[a_place.first][a_place.second];
                taken[a] = true;
            }
        }

        A_length += a_cost;
        B_length += b_cost;

        list<int>::iterator it = A.begin();
        advance(it,a_place.second);
        A.insert(it,a);
        
        it = B.begin();
        advance(it,b_place.second);
        B.insert(it,b);

        

    }

    A_cycle = A;
    B_cycle = B;
}

Greedy_cycle::Greedy_cycle(string file)
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

Greedy_cycle::~Greedy_cycle()
{
}

