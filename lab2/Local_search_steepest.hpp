#ifndef _STEEPEST
#define LS_STEEPEST

#define VERTEX_NEIGHBORHOOD 1667
#define EDGE_NEIGHBORHOOD 1666

#include "Local_search_abstract.hpp"

class Local_search_steepest: private Local_search_abstract
{
private:
    /* data */
    int neighborhood_type;
    pair<int,int> get_adjacent_vertex(vector<int> v, int index);
    void run() override;
public:
    Local_search_steepest(Cycle_abstract *cycle, int _neighborhood_type = VERTEX_NEIGHBORHOOD);
    ~Local_search_steepest();
};

pair<int,int>  Local_search_steepest::get_adjacent_vertex(vector<int> v, int index)
{
    pair<int,int> ret;

    if(index==0)
    {
        ret.first = A_cycle.back();
        ret.second = A_cycle[index+1];
    }
    else if (index==A_cycle.size()-1)
    {
        ret.first = A_cycle[index-1];
        ret.second = B_cycle[0];
    }
    else
    {
        ret.first = A_cycle[index-1];
        ret.second = B_cycle[index+1];
    }

    return ret;
}

void Local_search_steepest::run()
{
    // szukanie zamiany wierzcholkow miedzy trasami

    int min_len = 0;
    pair<int,int> min_change = make_pair(-1,-1); 
    for(int a = 0; a < A_cycle.size(); a++)
    {
        pair<int,int> neigh_A = get_adjacent_vertex(A_cycle, a);

        for(int b = 0; b < B_cycle.size(); b++)
        {
            pair<int,int> neigh_B = get_adjacent_vertex(B_cycle, b);
            
            int new_len = 0;

            new_len -= matrix[neigh_A.first][A_cycle[a]] + matrix[A_cycle[a]][neigh_A.second];
            new_len -= matrix[neigh_B.first][B_cycle[b]] + matrix[B_cycle[b]][neigh_B.second];

            new_len += matrix[neigh_A.first][B_cycle[b]] + matrix[B_cycle[b]][neigh_A.second];
            new_len += matrix[neigh_B.first][A_cycle[a]] + matrix[A_cycle[a]][neigh_B.second];

            if(min_change.first == -1 || (min_len > new_len && min_change.first>-1))
            {
                min_change = make_pair(a,b);
                min_len = new_len;
            }

        }
    }

    int intercycle_len = min_len;
    pair<int,int> intercycle_change = min_change;

    //innercycle sasiedztwo wierzcholki
    
    min_len = 0;
    min_change = make_pair(-1,-1);
    int where_change;
    if (neighborhood_type == VERTEX_NEIGHBORHOOD)
        {
        for(int cycle_index = 0; cycle_index<2;cycle_index++){
            vector<int>& cycle = (cycle_index==0) ? A_cycle : B_cycle;

            for(int a = 0; a < cycle.size(); a++)
            {
                pair<int,int> neigh_A = get_adjacent_vertex(cycle, a);
                for(int b = a+1; a < cycle.size(); a++)
                {
                    pair<int,int> neigh_B = get_adjacent_vertex(B_cycle, b);
                    
                    int new_len = 0;

                    new_len -= matrix[neigh_A.first][cycle[a]] + matrix[cycle[a]][neigh_A.second];
                    new_len -= matrix[neigh_B.first][cycle[b]] + matrix[cycle[b]][neigh_B.second];

                    new_len += matrix[neigh_A.first][cycle[b]] + matrix[cycle[b]][neigh_A.second];
                    new_len += matrix[neigh_B.first][cycle[a]] + matrix[cycle[a]][neigh_B.second];

                    if(min_change.first == -1 || (min_len > new_len && min_change.first>-1))
                    {
                        min_change = make_pair(a,b);
                        min_len = new_len;
                        where_change = cycle_index;
                    }

                }
            }
        }
        // porownanie i zamiana
        if(intercycle_len < min_len)
        {
            swap(A_cycle[intercycle_change.first],B_cycle[intercycle_change.second]);
        }
        else
        {
            if(where_change == 0)
                swap(A_cycle[min_change.first],A_cycle[min_change.second]);
            else
                swap(B_cycle[min_change.first],B_cycle[min_change.second]);
        }
    }
    else if(neighborhood_type == EDGE_NEIGHBORHOOD)
    {
        //TODO EDGE NEIGHBORHOOD
    }

}

Local_search_steepest::Local_search_steepest(Cycle_abstract *cycle, int _neighborhood_type) : Local_search_abstract(cycle)
{
    neighborhood_type = _neighborhood_type;
    run();
}

Local_search_steepest::~Local_search_steepest()
{
}


#endif