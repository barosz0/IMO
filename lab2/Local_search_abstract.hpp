#ifndef LS_ABSTRACT
#define LS_ABSTRACT

#include "Greedy_2regret_cycle.hpp"
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


    virtual void run() = 0;
    
public:
    Local_search_abstract(Cycle_abstract *cycle);
    ~Local_search_abstract();
};

Local_search_abstract::Local_search_abstract(Cycle_abstract *cycle)
{
    orginal_cycle = cycle;
    A_cycle = vector<int>(A_cycle.size());
    // A_cycle = cycle->A_cycle;
    std::copy(cycle->A_cycle.begin(), cycle->A_cycle.end(), A_cycle.begin());
    
    B_cycle = vector<int>(B_cycle.size());
    // B_cycle = cycle->B_cycle;
    std::copy(cycle->B_cycle.begin(), cycle->B_cycle.end(), B_cycle.begin());

    A_length = cycle->A_length;
    B_length = cycle->B_length;

    matrix = cycle -> get_matrix();
    moves = 0;
}

Local_search_abstract::~Local_search_abstract()
{
}

#endif