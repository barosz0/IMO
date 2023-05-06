#ifndef ITERATED_LOCAL_SEARCH_SMALL
#define ITERATED_LOCAL_SEARCH_SMALL

#include "Local_search_abstract.hpp"
#include "Memory_search.hpp" //MOVE DATA
#include <algorithm>
#include <random>
#include <numeric>

class Iterated_local_search_small: public Local_search_abstract
{
private:
    std::random_device r;
    std::default_random_engine rng = std::default_random_engine(r());
    int max_running_time;
    int moves_per_iteration;

    void run() override;
    void make_random_moves(int n, std::vector<int> &A, std::vector<int> &B);
    void make_move(move_data move, std::vector<int> &A, std::vector<int> &B);
public:
    Iterated_local_search_small(Cycle_abstract *cycle, int max_running_time_, int moves_per_iteration_);
    ~Iterated_local_search_small();
};

void Iterated_local_search_small::make_random_moves(int n, std::vector<int> &A, std::vector<int> &B)
{
    std::uniform_real_distribution<double> distribution;
    for(int i=0;i<n;i++)
    {
        
        move_data move;
        if(distribution(rng)>0.5)
        {
            move.type = INNER_CHANGE;
            move.which_cycle = (distribution(rng)>0.5) ? 0 : 1;
            vector<int>& cycle = (move.which_cycle==0) ? A : B;

            std::uniform_int_distribution<int> myUnifIntDist(0, A_cycle.size()-1);

            int a = myUnifIntDist(rng);
            int b = myUnifIntDist(rng);
            // printf("los a: %d b: %d\n", a, b);

            while (a==b)
            {
                a = myUnifIntDist(rng);
                b = myUnifIntDist(rng);
            }
            
            move.a = cycle[min(a,b)];
            move.b = cycle[max(a,b)];

            move.neigh_A = get_adjacent_vertex(cycle, a);
            move.neigh_B = get_adjacent_vertex(cycle, b);
        }
        else
        {
            move.type = OUTER_CHANGE;
            std::uniform_int_distribution<int> myUnifIntDistA(0, A.size()-1);
            std::uniform_int_distribution<int> myUnifIntDistB(0, B.size()-1);

            int a = myUnifIntDistA(rng);
            int b = myUnifIntDistB(rng);
            // printf("los a: %d b: %d\n", a, b);

            move.a = A[a];
            move.b = B[b];
        }
        make_move(move, A,B);
        
    }
    // exit(2);
}

void Iterated_local_search_small::make_move(move_data move, std::vector<int> &A, std::vector<int> &B)
{
    // printf("move: a: %d b: %d type: %d\n", move.a,move.b,move.type);
    if(move.type == OUTER_CHANGE)
    {
        // printf("outer\n");
        int index_a = getIndex(A,move.a);
        int index_b = getIndex(B,move.b);
        // printf("Swap %d %d\n",index_a, index_b);
        swap(A[index_a],B[index_b]);
    }

    if(move.type == INNER_CHANGE)
    {
        // printf("inner\n");
        vector<int>& cycle = (move.which_cycle==0) ? A : B;
        vector<int>& other_cycle = (move.which_cycle==0) ? B : A;

        int index_a = getIndex(cycle,move.a);
        int index_b = getIndex(cycle,move.b);

        // printf("reverse %d %d\n", index_a+1, index_b+1);
        reverse(cycle.begin()+index_a+1,cycle.begin()+index_b+1);

    }
    // printf("move done\n");
}

void Iterated_local_search_small::run()
{
    std::vector<int> A_copy = A_cycle;
    std::vector<int> B_copy = B_cycle;

    auto start = std::chrono::high_resolution_clock::now();
    while(true)
    {
        
        make_random_moves(moves_per_iteration, A_copy, B_copy);
        Memory_search LS(matrix, A_copy, B_copy);
        if(LS.get_length() < get_length())
        {
            A_cycle = LS.get_A_cycle();
            B_cycle = LS.get_B_cycle();
            recalculate_length();
            printf("new len:%d\n", get_length());

        }


        auto end = std::chrono::high_resolution_clock::now(); // pobranie aktualnego czasu
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); // obliczenie czasu jaki minął od startu
        
        if (elapsed_time >= max_running_time) // sprawdzenie, czy czas trwania został osiągnięty
        {
            break;
        }
    }
}

Iterated_local_search_small::Iterated_local_search_small(Cycle_abstract *cycle, int max_running_time_ = 10000, int moves_per_iteration_=10) : Local_search_abstract(cycle)
{
    max_running_time = max_running_time_;
    moves_per_iteration = moves_per_iteration_;
    run();
}

Iterated_local_search_small::~Iterated_local_search_small()
{
}

#endif