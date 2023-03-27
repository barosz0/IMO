#include "Local_search_abstract.hpp"
#include <algorithm>
#include <random>
#include <numeric>
#include <chrono>

#ifndef LS_RANDOM
#define LS_RANDOM

#ifndef VERTEX_NEIGHBORHOOD
#define VERTEX_NEIGHBORHOOD 1667
#endif

#ifndef EDGE_NEIGHBORHOOD
#define EDGE_NEIGHBORHOOD 1666
#endif

class Local_search_random : public Local_search_abstract
{
private:
    /* data */
    std::random_device r;
    std::default_random_engine rng = std::default_random_engine(r());
    void run() override;
    bool make_step();
    int max_running_time;
public:
    Local_search_random(Cycle_abstract *cycle, int max_running_time);
    ~Local_search_random();
    int minimum = 100000000;
};

void Local_search_random::run()
{
    auto start = std::chrono::high_resolution_clock::now(); // pobranie czasu startu
    while (true)
    {
        
        

        make_step();
        if (get_length()<minimum)
            minimum = get_length();
        moves++;

        auto end = std::chrono::high_resolution_clock::now(); // pobranie aktualnego czasu
        auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); // obliczenie czasu jaki minął od startu

        if (elapsed_time >= this->max_running_time) // sprawdzenie, czy czas trwania został osiągnięty
        {
            break;
        }
    }

    //while(make_step())moves++;// cout << moves++ << ". " << get_length() << endl;
}

bool Local_search_random::make_step()
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

    uniform_int_distribution<size_t> u (0, 4);

    int move = u(rng);


    //inter chenge
    if(move ==0)
    {
        int a = shuffle_A_cycle[0];
        int b = shuffle_B_cycle[0];

        swap(A_cycle[a],B_cycle[b]);
    }

        // inner change VERTEX_NEIGHBORHOOD A
    if(move == 1 )
    {
        
        int a = shuffle_A_cycle[0];
        int b = shuffle_A_cycle[0];

        swap(A_cycle[a],A_cycle[b]);
    }
    // inner change VERTEX_NEIGHBORHOOD B
    if(move == 2 )
    {
        
        int a = shuffle_B_cycle[0];
        int b = shuffle_B_cycle[0];

        swap(B_cycle[a],B_cycle[b]);
    }
    // inner change edge_NEIGHBORHOOD A
    if(move == 3 )
    {
        
        int a = shuffle_A_cycle[0];
        int b = shuffle_A_cycle[0];

        reverse(A_cycle.begin()+a+1,A_cycle.begin()+b+1 );
    }
    // inner change edge_NEIGHBORHOOD B
    if(move == 4 )
    {
        
        int a = shuffle_B_cycle[0];
        int b = shuffle_B_cycle[0];

        reverse(B_cycle.begin()+a+1,B_cycle.begin()+b+1 );
    }
        return true;
}

Local_search_random::Local_search_random(Cycle_abstract *cycle, int max_running_time = 10000): Local_search_abstract(cycle)
{
    
    this-> max_running_time = max_running_time;
    run();
    recalculate_length();
}

Local_search_random::~Local_search_random()
{
}

#endif