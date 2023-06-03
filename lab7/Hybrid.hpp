#ifndef HYBRID_EVOLUTIONARY
#define HYBRID_EVOLUTIONARY

#include "Local_search_abstract.hpp"
#include "Greedy_2regret_cycle.hpp"
#include "Memory_search.hpp"
#include "Random_cycle.hpp"
#include <random>

#ifndef STRUCT_SOLUTION
#define STRUCT_SOLUTION
struct solution
{
    std::vector<int> A_cycle;
    std::vector<int> B_cycle;
    int score;

    solution(Local_search_abstract *LS)
    {
        A_cycle = LS->get_A_cycle();
        B_cycle = LS->get_B_cycle();

        score = LS->get_length();
    }

    solution(Cycle_abstract *cycle)
    {
        A_cycle = vector<int>(cycle->A_cycle.size());
        std::copy(cycle->A_cycle.begin(), cycle->A_cycle.end(), A_cycle.begin());

        B_cycle = vector<int>(cycle->B_cycle.size());
        std::copy(cycle->B_cycle.begin(), cycle->B_cycle.end(), B_cycle.begin());

        score = cycle -> get_length();
    }
    solution()
    {
        score = -1;
    }
};
#endif


class Hybrid : public Local_search_abstract
{
private:
    std::random_device r;
    std::default_random_engine rng = std::default_random_engine(r());

    std::vector<solution> population;
    void fill_starting_population(int n);
    bool check_if_score_in_population(int score);
    bool check_if_edge_in_cycle(int a, int b, vector<int> &cycle);
    int check_if_enter_population(solution sol);
    solution make_new_solution();
    solution combine_solutions(solution A, solution B);

    solution find_best_solution();

    solution best_solution;
    decltype(std::chrono::high_resolution_clock::now()) start_time;

    int population_size;
    int max_running_time;
    bool do_local_search;
    string filename;
    
    void run() override;
public:
    Hybrid(string filename_, int max_running_time_, bool do_local_search_, int population_size_);
    ~Hybrid();
};

solution Hybrid::find_best_solution()
{
    solution ret = population[0];

    for(int i=0; i<population.size();i++)
    {
        if(ret.score > population[i].score)
        {
            ret = population[i];
        }
    }
    return ret;
}

bool Hybrid::check_if_edge_in_cycle(int a, int b, vector<int> &cycle)
{
    for(int i = 0; i < cycle.size()-1; i++)
    {
        if(cycle[i] == a)
            if(cycle[i+1] == b)
                return true;
    }
    if(cycle[cycle.size()-1] == a)
        if(cycle[0] == b)
            return true;
    
    return false;
}

int Hybrid::check_if_enter_population(solution sol)
{
    int position = -1;
    for(int i=0; i<population.size();i++)
    {
        if(population[i].score == sol.score) return -1;
        if(population[i].score > sol.score)
        {
            if(position == -1)
                position = i;
            else if(population[i].score < population[position].score)
                position = i;

        }
    }
    return position;
}


solution Hybrid::combine_solutions(solution A, solution B)
{
    // printf("combine solution\n");
    vector<int> new_A_cycle;
    bool second_push = false;
    for(int i = 0; i < A.A_cycle.size()-1; i++)
    {
        if(check_if_edge_in_cycle(A.A_cycle[i],A.A_cycle[i+1], B.A_cycle))
        {
            new_A_cycle.push_back(A.A_cycle[i]);
            second_push = true;
        }
        else if(second_push)
        {
            second_push = false;
            new_A_cycle.push_back(A.A_cycle[i]);
        }
    }

    if(check_if_edge_in_cycle(A.A_cycle[A.A_cycle.size()-1],A.A_cycle[0], B.A_cycle))
    {
        new_A_cycle.push_back(A.A_cycle[A.A_cycle.size()-1]);
        second_push = true;
        if(new_A_cycle[0] != A.A_cycle[0])
            new_A_cycle.push_back(A.A_cycle[0]);
    }
    else if(second_push)
    {
        second_push = false;
        new_A_cycle.push_back(A.A_cycle[A.A_cycle.size()-1]);
    }
    // printf("first half\n");
    vector<int> new_B_cycle;
    second_push = false;
    for(int i = 0; i < A.B_cycle.size()-1; i++)
    {
        if(check_if_edge_in_cycle(A.B_cycle[i],A.B_cycle[i+1], B.B_cycle))
        {
            new_B_cycle.push_back(A.B_cycle[i]);
            second_push = true;
        }
        else if(second_push)
        {
            new_B_cycle.push_back(A.B_cycle[i]);
        }
    }

    if(check_if_edge_in_cycle(A.B_cycle[A.B_cycle.size()-1],A.B_cycle[0], B.B_cycle))
    {
        new_B_cycle.push_back(A.B_cycle[A.B_cycle.size()-1]);
        second_push = true;
        if(new_B_cycle[0] != A.B_cycle[0])
            new_B_cycle.push_back(A.B_cycle[0]);
    }
    else if(second_push)
    {
        second_push = false;
        new_B_cycle.push_back(A.B_cycle[A.B_cycle.size()-1]);
    }
    // printf("Making cycle %d and %d\n", new_A_cycle.size(), new_B_cycle.size());
    // if(new_A_cycle.size() == 0 || new_B_cycle.size() == 0)
    //     printf("<++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    Greedy_2regret_cycle cycle = Greedy_2regret_cycle(matrix,coords, new_A_cycle,new_B_cycle);

    if(do_local_search){
        Memory_search MS = Memory_search(&cycle);
        return solution(&MS);
    }
    // printf("Combine end\n");

    return solution(&cycle);
}

solution Hybrid::make_new_solution()
{
    std::uniform_int_distribution<int> myUnifIntDist(0,population.size()-1);
    int a = myUnifIntDist(rng);
    int b = myUnifIntDist(rng);
    while (a==b) b = myUnifIntDist(rng);

    solution A = population[a], B = population[b];
    return combine_solutions(A,B);

}

void Hybrid::run()
{
    // printf("run\n");
    // auto start = std::chrono::high_resolution_clock::now();
    auto start = start_time;

    while(true)
    {
        moves++;
        solution s = make_new_solution();
        int index = check_if_enter_population(s);
        // printf("index: %d\n", index);
        if(index != -1)
        {
            // printf("Nowy czlonek spoleczenstwa (score = %d)\n", s.score);
            population[index] = s;
        }

        auto end = std::chrono::high_resolution_clock::now(); // pobranie aktualnego czasu
        auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); // obliczenie czasu jaki minął od startu
        if (elapsed_time >= max_running_time) // sprawdzenie, czy czas trwania został osiągnięty
            {
                break;
            }
        // printf("while end;\n");
    }
}


void Hybrid::fill_starting_population(int n)
{
    while(population.size() < n)
    {   
        Random_cycle cycle = Random_cycle(filename);
        Memory_search LS = Memory_search(&cycle);   
        if(!check_if_score_in_population(LS.get_length()))
        {
            population.push_back(solution(&LS));
        }

    }
}

bool Hybrid::check_if_score_in_population(int score)
{
    for(int i = 0; i<population.size(); i++)
    {
        if(population[i].score == score)
            return true;
    }
    return false;
}

Hybrid::Hybrid(string filename_, int max_running_time_, bool do_local_search_ = true, int population_size_ = 20)
{
    do_local_search = do_local_search_;
    population_size = population_size_;
    filename = filename_;
    max_running_time = max_running_time_;

    parser p(filename_);
    matrix = p.matrix;
    coords = p.coords;

    filename = filename_;

    start_time = std::chrono::high_resolution_clock::now();

    fill_starting_population(population_size);
    best_solution = find_best_solution();
    // printf("Startowa najlepsza: %d\n",best_solution.score);
    run();
    best_solution = find_best_solution();
    // printf("Koncowa najlepsza: %d\n",best_solution.score);
    A_cycle = best_solution.A_cycle;
    B_cycle = best_solution.B_cycle;
    recalculate_length();
}


Hybrid::~Hybrid()
{
    
}

#endif
