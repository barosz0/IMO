#ifndef ITERATED_LOCAL_SEARCH_BIG
#define ITERATED_LOCAL_SEARCH_BIG

#include "Local_search_abstract.hpp"
#include "Greedy_2regret_cycle.hpp"
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>

class Iterated_local_search_big: public Local_search_abstract
{
private:
    std::random_device r;
    std::default_random_engine rng = std::default_random_engine(r());
    int max_running_time;
    int to_delete;
    void make_delete(std::vector<int> &A, std::vector<int> &B);
    void run() override;
public:
    Iterated_local_search_big(Cycle_abstract *cycle, int max_running_time_);
    ~Iterated_local_search_big();
};

void Iterated_local_search_big::make_delete(std::vector<int> &A, std::vector<int> &B)
{
    std::uniform_real_distribution<double> distribution;
    std::uniform_int_distribution<int> myUnifIntDist(0, A.size() + B.size()-1);
    vector<bool> delete_flag(A.size() + B.size());

    int temp = to_delete;
    int delete_index;
    // printf("temp: %d\n", temp);
    while(temp > 0)
    {
        
        while(delete_index) delete_index = myUnifIntDist(rng);

        delete_flag[delete_index] = true;
        temp-=1;
        while (distribution(rng) < 0.8 && temp > 0)
        {
            delete_index++;
            if(delete_index == A.size() + B.size()) delete_index = (A.size() + B.size())/2;
            if(delete_index == (A.size() + B.size())/2-1) delete_index = 0;
            delete_flag[delete_index] = true;
            temp-=1;
        }
        // printf("temp: %d\n", temp);
    }
    std::vector<int> A_new;
    std::vector<int> B_new;

    for(int i = 0; i< delete_flag.size(); i++)
    {
        if(!delete_flag[i])
        {
            if(i < (A.size() + B.size())/2)
            {
                A_new.push_back(A[i]);
            }
            else
            {
                // printf("%d: %d\n",i-(A.size() + B.size())/2,B[i-(A.size() + B.size())/2] );
                B_new.push_back(B[i-(A.size() + B.size())/2]);
            }
        }
    }
    A = A_new;
    B = B_new;
   
}

void Iterated_local_search_big::run()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> A_copy = A_cycle;
    std::vector<int> B_copy = B_cycle;

    while(true)
    {
        auto end = std::chrono::high_resolution_clock::now(); // pobranie aktualnego czasu
        auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); // obliczenie czasu jaki minął od startu
        
        make_delete(A_copy,B_copy);
        // printf("Greedy 2\n");
        Greedy_2regret_cycle cycle(matrix, orginal_cycle->get_coords(), A_copy, B_copy);
        // printf("Greedy 2 - end\n");

        A_copy = vector<int>(cycle.A_cycle.size());
        std::copy(cycle.A_cycle.begin(), cycle.A_cycle.end(), A_copy.begin());

        B_copy = vector<int>(cycle.B_cycle.size());
        std::copy(cycle.B_cycle.begin(), cycle.B_cycle.end(), B_copy.begin());

        // printf("len: %d\nnew len: %d\n", get_length(),cycle.get_length());
        if(cycle.get_length() < get_length())
        {
            A_cycle = A_copy;
            B_cycle = B_copy;
            recalculate_length();
            printf("new len:%d\r", get_length());

        }

        if (elapsed_time >= max_running_time) // sprawdzenie, czy czas trwania został osiągnięty
        {
            break;
        }
    }
}

Iterated_local_search_big::Iterated_local_search_big(Cycle_abstract *cycle, int max_running_time_ = 10000) : Local_search_abstract(cycle)
{
    max_running_time = max_running_time_;
    to_delete = 0.2*(cycle->get_coords().size());
    run();
}

Iterated_local_search_big::~Iterated_local_search_big()
{
    
}

#endif
