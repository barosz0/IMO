#include "Cycle_abstract.hpp"
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>



class Random_cycle: public Cycle_abstract
{
private:
    int max_running_time;    

    
    void run() override;
    int next_permutation();
    void random_permintation();
    
    int get_element(list<int>* _list, int index);

public:
    Random_cycle(string file);
    Random_cycle(string file, int a);
    ~Random_cycle();
};


void Random_cycle::random_permintation()
{
    int nodes = this->coords.size();
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    vector<int> numbers(nodes);
    std::iota (std::begin(numbers), std::end(numbers), 0);
    shuffle(numbers.begin(), numbers.end(), rng);

   for(int i=0; i<coords.size();i++)
   {    
        if (i<coords.size()/2)
            this->A_cycle.push_back(numbers[i]);
        else
            this->B_cycle.push_back(numbers[i]);
   }

}


int Random_cycle::next_permutation()
{
   
   
    
    for(int i = 0; i < coords.size(); i++)
    {
        if(taken[i])
            continue;
        
    }
    return 0;

}

void Random_cycle::run()
{
    list<int>* A = &A_cycle;
    list<int>* B = &B_cycle;

    int* a_len = &A_length;
    int* b_len = &B_length;

    random_permintation();

    this->A_length = this->get_cycle_length(A_cycle);
    this->B_length = this->get_cycle_length(B_cycle);

    /*auto start = std::chrono::high_resolution_clock::now(); // pobranie czasu startu
    while (true)
    {
        
        


        auto end = std::chrono::high_resolution_clock::now(); // pobranie aktualnego czasu
        auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); // obliczenie czasu jaki minął od startu

        if (elapsed_time >= this->max_running_time) // sprawdzenie, czy czas trwania został osiągnięty
        {
            break;
        }
    }*/

    
    /*while(A->size() + B->size() < coords.size())
    {
        
        *a_len += next_pick(A,B);
        swap(A,B);
        swap(a_len,b_len);
            
        // printf("\rPozostalo: %d   ", coords.size() - (A->size() + B->size()));
        // cout<< A->size() + B->size() << endl;
    }*/
    // cout << endl;
}



Random_cycle::Random_cycle(string file) : Cycle_abstract(file)
{
    run();
    //cout << "Random cycle length: "<< this->get_length()<<endl;
}

Random_cycle::Random_cycle(string file, int number) : Cycle_abstract(file)
{
    run();
    //cout << "Random cycle length: "<< this->get_length()<<endl;
}

Random_cycle::~Random_cycle()
{}

