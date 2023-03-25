#ifndef _STEEPEST
#define LS_STEEPEST

#define VERTEX_NEIGHBORHOOD 1667
#define EDGE_NEIGHBORHOOD 1666

#include "Local_search_abstract.hpp"

class Local_search_steepest: public Local_search_abstract
{
private:
    /* data */
    int neighborhood_type;
    pair<int,int> get_adjacent_vertex(vector<int> v, int index);
    void run() override;
    bool make_step();
    
public:

    Local_search_steepest(Cycle_abstract *cycle, int _neighborhood_type = VERTEX_NEIGHBORHOOD);
    ~Local_search_steepest();
};



pair<int,int>  Local_search_steepest::get_adjacent_vertex(vector<int> v, int index)
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

void Local_search_steepest::run()
{
    recalculate_length();
    while(make_step());// cout << moves++ << endl;
}

bool Local_search_steepest::make_step()
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
                for(int b = a+1; b < cycle.size(); b++)
                {
                    pair<int,int> neigh_B = get_adjacent_vertex(cycle, b);
                    
                    int new_len = 0;

                    new_len -= matrix[neigh_A.first][cycle[a]] + matrix[cycle[a]][neigh_A.second];
                    new_len -= matrix[neigh_B.first][cycle[b]] + matrix[cycle[b]][neigh_B.second];

                    if(neigh_A.second == cycle[b])
                    {
                        new_len += matrix[neigh_A.first][cycle[b]] + matrix[cycle[b]][cycle[a]];
                        new_len += matrix[cycle[a]][neigh_B.second] + matrix[cycle[a]][cycle[b]];
                        
                    }
                    else if(neigh_B.second == cycle[a])
                    {
                        new_len += matrix[cycle[b]][neigh_A.second] + matrix[cycle[b]][cycle[a]];
                        new_len += matrix[neigh_B.first][cycle[a]] + matrix[cycle[b]][cycle[a]];
                    }
                    else
                    {
                        new_len += matrix[neigh_A.first][cycle[b]] + matrix[cycle[b]][neigh_A.second];
                        new_len += matrix[neigh_B.first][cycle[a]] + matrix[cycle[a]][neigh_B.second];
                    }

                    if(min_change.first == -1 || (min_len > new_len && min_change.first>-1))
                    {
                        min_change = make_pair(a,b);
                        min_len = new_len;
                        where_change = cycle_index;
                        // cout<<"1. " << matrix[neigh_A.first][A_cycle[a]] << " " << matrix[A_cycle[a]][neigh_A.second] <<" " << a << " " << b << endl;
                        // cout<<"2. " << matrix[neigh_B.first][B_cycle[b]] << " " << matrix[B_cycle[b]][neigh_B.second] <<" " << a << " " << b << endl;
                        // cout<<"3. " << matrix[neigh_A.first][B_cycle[b]] << " " << matrix[B_cycle[b]][neigh_A.second] <<" " << a << " " << b << endl;
                        // cout<<"4. " << matrix[neigh_B.first][A_cycle[a]] << " " << matrix[A_cycle[a]][neigh_B.second] <<" " << a << " " << b << endl;

                    }

                }
            }
        }


        if(intercycle_len >= 0 && min_len >= 0)
            return false;

        // porownanie i zamiana
        if(intercycle_len < min_len)
        {
            int len = A_length + B_length;
            swap(A_cycle[intercycle_change.first],B_cycle[intercycle_change.second]);
            recalculate_length();
        }
        else
        {
            
            if(where_change == 0){
                swap(A_cycle[min_change.first],A_cycle[min_change.second]);
                }
            else{
                swap(B_cycle[min_change.first],B_cycle[min_change.second]);
                }
        }
    }
    else if(neighborhood_type == EDGE_NEIGHBORHOOD)
    {
        
        // krawedz indeksuje numerem wierzcholka
        for(int cycle_index = 0; cycle_index<2;cycle_index++){
            vector<int>& cycle = (cycle_index==0) ? A_cycle : B_cycle;
            for(int a = 0; a < cycle.size(); a++)
            {
                for(int b = a+1; b < cycle.size(); b++)
                {
                    int new_len = 0;

                    new_len -= matrix[cycle[a]][cycle[a+1]];
                    if(b + 1 < cycle.size())
                        new_len -= matrix[cycle[b]][cycle[b+1]];
                    else // ostatnia krawedz
                        new_len -= matrix[cycle[b]][cycle[0]];

                    new_len += matrix[cycle[a]][cycle[b]];
                    if(b + 1 < cycle.size())
                        new_len += matrix[cycle[a+1]][cycle[b+1]];
                    else
                        new_len += matrix[cycle[a+1]][cycle[0]];

                    if(min_change.first == -1 || (min_len > new_len && min_change.first>-1))
                    {
                        min_change = make_pair(a,b);
                        min_len = new_len;
                        where_change = cycle_index;
                    }
                }
            }
        }
        // cout<< "Zamiany: " << intercycle_len << " " << min_len << endl;
        if(intercycle_len >= 0 && min_len >= 0)
            return false;
        
        if(intercycle_len < min_len)
        {
            swap(A_cycle[intercycle_change.first],B_cycle[intercycle_change.second]);
        }
        else
        {
            if(where_change == 0)
            {
                // cout<< "Zamiana2A " << min_change.first << " " <<min_change.second << endl;
                reverse(A_cycle.begin()+min_change.first+1,A_cycle.begin()+min_change.second+1 );
            }
            else
            {
                // cout<< "Zamiana2B " << min_change.first << " " <<min_change.second << endl;
                reverse(B_cycle.begin()+min_change.first+1,B_cycle.begin()+min_change.second+1 );
            }
            
        }

        //TODO porownanie i zamiana
    }
    return true;
}

Local_search_steepest::Local_search_steepest(Cycle_abstract *cycle, int _neighborhood_type) : Local_search_abstract(cycle)
{
    neighborhood_type = _neighborhood_type;

    run();
    recalculate_length();
}

Local_search_steepest::~Local_search_steepest()
{
}


#endif