#ifndef LS_ABSTRACT
#define LS_ABSTRACT

#include "Cycle_abstract.hpp"


class Local_search_abstract
{
protected:
    /* data */
    vector<vector<int>> matrix;
    Cycle_abstract* orginal_cycle;
    std::vector<int> A_cycle;
    std::vector<int> B_cycle;
    int A_length;
    int B_length;
    int moves;
    pair<int,int> get_adjacent_vertex(vector<int> v, int index);

    void recalculate_length();
    virtual void run() = 0;
    
public:
    int get_length();
    vector<pair<double, double>> get_cycle_coords(vector<int> cycle);

    vector<pair<double, double>> get_A_coords();
    vector<pair<double, double>> get_B_coords();

    std::vector<int> get_A_cycle();
    std::vector<int> get_B_cycle();
    
    Local_search_abstract(Cycle_abstract *cycle);
    Local_search_abstract();
    ~Local_search_abstract();
    
};

std::vector<int> Local_search_abstract::get_A_cycle()
{
    return A_cycle;
}

std::vector<int> Local_search_abstract::get_B_cycle()
{
    return B_cycle;
}

pair<int,int> Local_search_abstract::get_adjacent_vertex(vector<int> v, int index)
{
    pair<int,int> ret;

    if(index==0)
    {
        ret.first = v.back();
        ret.second = v[index+1];
    }
    else if (index==v.size()-1)
    {
        ret.first = v[index-1];
        ret.second = v[0];
    }
    else
    {
        ret.first = v[index-1];
        ret.second = v[index+1];
    }

    return ret;
}

vector<pair<double, double>> Local_search_abstract::get_A_coords()
{
    return get_cycle_coords(A_cycle);
}

vector<pair<double, double>> Local_search_abstract::get_B_coords()
{
    return get_cycle_coords(B_cycle);
}

vector<pair<double, double>> Local_search_abstract::get_cycle_coords(vector<int> cycle)
{
    vector<pair<double, double>> cycle_coords;
    vector<pair<double, double>> coords = orginal_cycle->get_coords();
    
    for (int i = 0; i < cycle.size(); i++)
        cycle_coords.push_back(coords[cycle[i]]);

    return (cycle_coords);
}

void Local_search_abstract::recalculate_length()
{
    if(A_cycle.size() > 0)
    {    
        int len = 0;
        for(int a = 0; a < A_cycle.size()-1; a++)
        {
            // printf("a: %d\n",a);
            len += matrix[A_cycle[a]][A_cycle[a+1]];
        }
        len += matrix[A_cycle.back()][A_cycle[0]];
        A_length = len;
    }
    else
    {
        A_length = 0;
    }

    if(B_cycle.size() > 0)
    {    
        int  len = 0;
        for(int a = 0; a < B_cycle.size()-1; a++)
        {
            // printf("b: %d\n",a);
            len += matrix[B_cycle[a]][B_cycle[a+1]];
        }
        len += matrix[B_cycle.back()][B_cycle[0]];
        B_length = len;
    }
    else
    {
        B_length = 0;
    }
}

int Local_search_abstract::get_length()
{
    recalculate_length();
    return A_length + B_length;
}

Local_search_abstract::Local_search_abstract(Cycle_abstract *cycle)
{
    
    orginal_cycle = cycle;
    A_cycle = vector<int>(cycle->A_cycle.size());
    // A_cycle = cycle->A_cycle;
    std::copy(cycle->A_cycle.begin(), cycle->A_cycle.end(), A_cycle.begin());
    
    B_cycle = vector<int>(cycle->B_cycle.size());
    // B_cycle = cycle->B_cycle;
    std::copy(cycle->B_cycle.begin(), cycle->B_cycle.end(), B_cycle.begin());

    A_length = cycle->A_length;
    B_length = cycle->B_length;

    matrix = cycle -> get_matrix();
    moves = 0;
}

Local_search_abstract::Local_search_abstract(){}

Local_search_abstract::~Local_search_abstract()
{
}

#endif