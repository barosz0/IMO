#ifndef CYCLE_ABSTRACT
#define CYCLE_ABSTRACT

#include "parser.hpp"
#include <cstdlib>
#include <list>
#include <algorithm>

class Cycle_abstract
{
protected:
    vector<vector<int>> matrix;
    vector<pair<double, double>> coords; 
    vector<bool> taken;

    void choose_random_starting(int &a,int &b);

    virtual void run() = 0;

public:
    list<int> A_cycle;
    list<int> B_cycle;
    int A_length;
    int B_length;
    int get_cycle_length(list<int> cycle);
    vector<pair<double, double>> get_cycle_coords(list<int> cycle);
    vector<vector<int>> get_matrix();
    vector<pair<double, double>> get_coords();
    int get_length();
    
    Cycle_abstract(string file);
    Cycle_abstract(string file, int a);
    Cycle_abstract(vector<vector<int>> matrix_, vector<pair<double, double>> coords_, std::vector<int> A, std::vector<int> B);
    ~Cycle_abstract();
};

int Cycle_abstract::get_length()
{
    return get_cycle_length(A_cycle) + get_cycle_length(B_cycle);
}

vector<vector<int>> Cycle_abstract::get_matrix()
{
    return matrix;
}

vector<pair<double, double>> Cycle_abstract::get_coords()
{
    return coords;
}

vector<pair<double, double>> Cycle_abstract::get_cycle_coords(list<int> cycle)
{
    vector<pair<double, double>> cycle_coords;
    list<int>::iterator i;
    
    for (i = cycle.begin(); i != cycle.end(); ++i)
        cycle_coords.push_back(coords[*i]);

    return (cycle_coords);
}

int Cycle_abstract::get_cycle_length(list<int> cycle)
{
    list<int>::iterator it = cycle.begin();
    int pop = *it;
    int sum = 0;

    cycle.push_back(*it);
    it++;

    for (;it != cycle.end(); ++it){
        sum+=matrix[pop][*it];
        pop = *it;
        }
    return sum;
}

void Cycle_abstract::choose_random_starting(int &a, int &b)
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

Cycle_abstract::Cycle_abstract(string file)
{
    parser p(file);

    matrix = p.matrix;
    coords = p.coords;

    int a,b;

    choose_random_starting(a,b);

    // cout << "Random start a: "<< a << endl;

    A_cycle = list<int>{a};
    A_length = 0;

    B_cycle = list<int>{b};
    B_length = 0;

    taken = vector<bool>(coords.size());
    taken[b] = taken[a] = true;
}

Cycle_abstract::Cycle_abstract(string file, int a)
{
    parser p(file);

    matrix = p.matrix;
    coords = p.coords;

    int b = 0;
    for(int i = 1; i < coords.size(); i++)
    {
        if(matrix[a][b] < matrix[a][i])
        {
            b = i;
        }
    }

    A_cycle = list<int>{a};
    A_length = 0;

    B_cycle = list<int>{b};
    B_length = 0;

    taken = vector<bool>(coords.size());
    taken[b] = taken[a] = true;
}

Cycle_abstract::Cycle_abstract(vector<vector<int>> matrix_, vector<pair<double, double>> coords_, std::vector<int> A, std::vector<int> B)
{
    // printf("Abstract_start\n");
    matrix = matrix_;
    coords = coords_;



    A_cycle = list<int>(A.begin(), A.end());
    A_length = 0;

    B_cycle = list<int>(B.begin(), B.end());
    B_length = 0;
    taken = vector<bool>(coords.size());

    for(int i = 0; i<A.size();i++)
    {
        taken[A[i]]= true;
    }

    for(int i = 0; i<B.size();i++)
    {
        taken[B[i]]= true;
    }
}

Cycle_abstract::~Cycle_abstract()
{
}

#endif