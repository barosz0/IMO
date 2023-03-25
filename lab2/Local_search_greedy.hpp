#include "Local_search_abstract.hpp"
#include <algorithm>
#include <random>
#include <numeric>

#ifndef LS_GREEDY
#define LS_GREEDY

#ifndef VERTEX_NEIGHBORHOOD
#define VERTEX_NEIGHBORHOOD 1667
#endif

#ifndef EDGE_NEIGHBORHOOD
#define EDGE_NEIGHBORHOOD 1666
#endif

class Local_search_greedy : public Local_search_abstract
{
private:
    /* data */
    std::random_device r;
    std::default_random_engine rng = std::default_random_engine(r());
    int neighborhood_type;
    void run() override;
    bool make_step();
public:
    Local_search_greedy(Cycle_abstract *cycle, int _neighborhood_type = VERTEX_NEIGHBORHOOD);
    ~Local_search_greedy();
};

void Local_search_greedy::run()
{
    while(make_step()) cout << moves++ << ". " << get_length() << endl;
}

bool Local_search_greedy::make_step()
{
    vector<int> shuffle_A_cycle(A_cycle.size());
    std::iota (std::begin(shuffle_A_cycle), std::end(shuffle_A_cycle), 0);
    shuffle(begin(shuffle_A_cycle), end(shuffle_A_cycle), rng);

    vector<int> shuffle_B_cycle(B_cycle.size());
    std::iota (std::begin(shuffle_B_cycle), std::end(shuffle_B_cycle), 0);
    shuffle(begin(shuffle_B_cycle), end(shuffle_B_cycle), rng);

    
    int inter_a = 0;
    int inter_b = 0;

    int inner_Aa = 0;
    int inner_Ab = 1;

    int inner_Ba = 0;
    int inner_Bb = 1;

    uniform_int_distribution<size_t> u (0, 2);

    while(true)
    {
        // if(moves >= 28)
        // {
        //     cout<< "inter_a: " << inter_a << " | " << "inter_b: " << inter_b << endl;
        //     cout<< "inner_Aa: " << inner_Aa << " | " << "inner_Ab: " << inner_Ab << endl; 
        //     cout<< "inner_Ba: " << inner_Ba << " | " << "inner_Bb: " << inner_Bb << endl; 
        // }
        int move = u(rng);

        if(!(inter_a < A_cycle.size()) && !(inner_Aa < A_cycle.size()) && !(inner_Ba < B_cycle.size()))
            return false; // nie znaleziono lepszego

        //inter chenge
        if(move ==0 && inter_a < A_cycle.size())
        {
            int a = shuffle_A_cycle[inter_a];
            int b = shuffle_B_cycle[inter_b];

            pair<int,int> neigh_A = get_adjacent_vertex(A_cycle, a);
            pair<int,int> neigh_B = get_adjacent_vertex(B_cycle, b);
            int new_len = 0;

            new_len -= matrix[neigh_A.first][A_cycle[a]] + matrix[A_cycle[a]][neigh_A.second];
            new_len -= matrix[neigh_B.first][B_cycle[b]] + matrix[B_cycle[b]][neigh_B.second];

            new_len += matrix[neigh_A.first][B_cycle[b]] + matrix[B_cycle[b]][neigh_A.second];
            new_len += matrix[neigh_B.first][A_cycle[a]] + matrix[A_cycle[a]][neigh_B.second];

            if (new_len < 0)
            {
                cout << "Zamiana inter: " << a << " " << b << " " << new_len << endl;
                swap(A_cycle[a],B_cycle[b]);
                return true;
            }
            else
            {
                inter_b++;
                if(inter_b >= B_cycle.size())
                {
                    inter_a++;
                    inter_b=0;
                }
            }
        }
        
        // inter change VERTEX_NEIGHBORHOOD A
        if(move == 1 && inner_Aa < A_cycle.size())
        {
            
            int a = min(shuffle_A_cycle[inner_Aa],shuffle_A_cycle[inner_Ab]);
            int b = max(shuffle_A_cycle[inner_Aa],shuffle_A_cycle[inner_Ab]);

            pair<int,int> neigh_A = get_adjacent_vertex(A_cycle, a);
            pair<int,int> neigh_B = get_adjacent_vertex(A_cycle, b);

            int new_len = 0;
            

            new_len -= matrix[neigh_A.first][A_cycle[a]] + matrix[A_cycle[a]][neigh_A.second];
            new_len -= matrix[neigh_B.first][A_cycle[b]] + matrix[A_cycle[b]][neigh_B.second];

            if(neigh_A.second == A_cycle[b])
            {
                new_len += matrix[neigh_A.first][A_cycle[b]] + matrix[A_cycle[b]][A_cycle[a]];
                new_len += matrix[A_cycle[a]][neigh_B.second] + matrix[A_cycle[a]][A_cycle[b]];
                
            }
            else if(neigh_B.second == A_cycle[a])
            {
                new_len += matrix[A_cycle[b]][neigh_A.second] + matrix[A_cycle[b]][A_cycle[a]];
                new_len += matrix[neigh_B.first][A_cycle[a]] + matrix[A_cycle[b]][A_cycle[a]];
            }
            else
            {
                new_len += matrix[neigh_A.first][A_cycle[b]] + matrix[A_cycle[b]][neigh_A.second];
                new_len += matrix[neigh_B.first][A_cycle[a]] + matrix[A_cycle[a]][neigh_B.second];
            }

            if(new_len < 0)
            {
                cout << "Zamiana A: " << a << " " << b << " " << new_len << endl;
                swap(A_cycle[a],A_cycle[b]);
                return true;
            }

            inner_Ab++;
            if(inner_Ab >= A_cycle.size())
            {
                inner_Aa++;
                inner_Ab=inner_Aa+1;
                if(inner_Ab >= A_cycle.size()) inner_Aa = A_cycle.size();
            }
        }

        // inter change VERTEX_NEIGHBORHOOD B
        if(move == 2 && inner_Ba < B_cycle.size())
        {
            int a = min(shuffle_B_cycle[inner_Ba],shuffle_B_cycle[inner_Bb]);
            int b = max(shuffle_B_cycle[inner_Ba],shuffle_B_cycle[inner_Bb]);

            pair<int,int> neigh_A = get_adjacent_vertex(B_cycle, a);
            pair<int,int> neigh_B = get_adjacent_vertex(B_cycle, b);

            int new_len = 0;

            new_len -= matrix[neigh_A.first][B_cycle[a]] + matrix[B_cycle[a]][neigh_A.second];

            new_len -= matrix[neigh_B.first][B_cycle[b]] + matrix[B_cycle[b]][neigh_B.second];
            
            if(neigh_A.second == B_cycle[b])
            {
                new_len += matrix[neigh_A.first][B_cycle[b]] + matrix[B_cycle[b]][B_cycle[a]];
                new_len += matrix[B_cycle[a]][neigh_B.second] + matrix[B_cycle[a]][B_cycle[b]];
                
            }
            else if(neigh_B.second == B_cycle[a])
            {
                new_len += matrix[B_cycle[b]][neigh_A.second] + matrix[B_cycle[b]][B_cycle[a]];
                new_len += matrix[neigh_B.first][B_cycle[a]] + matrix[B_cycle[b]][B_cycle[a]];
            }
            else
            {
                new_len += matrix[neigh_A.first][B_cycle[b]] + matrix[B_cycle[b]][neigh_A.second];
                new_len += matrix[neigh_B.first][B_cycle[a]] + matrix[B_cycle[a]][neigh_B.second];
            }

            if(new_len < 0)
            {
                cout << "Zamiana B: " << a << " " << b <<" " << new_len << endl;
                swap(B_cycle[a],B_cycle[b]);
                return true;
            }

            inner_Bb++;
            if(inner_Bb >= B_cycle.size())
            {
                inner_Ba++;
                inner_Bb=inner_Ba+1;

                if(inner_Bb >= B_cycle.size()) inner_Ba = B_cycle.size();
            }
        }
        
        
    }
}

Local_search_greedy::Local_search_greedy(Cycle_abstract *cycle, int _neighborhood_type) : Local_search_abstract(cycle)
{
    neighborhood_type = _neighborhood_type;

    run();
    recalculate_length();
}

Local_search_greedy::~Local_search_greedy()
{
}

#endif