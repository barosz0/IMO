#include "Cycle_abstract.hpp"

#ifndef Greedy_2REGRET
#define Greedy_2REGRET
class Greedy_2regret_cycle: public Cycle_abstract
{
private:
    
    int calc_regret(vector<int> costs, int k = 2);
    
    void run() override;
    int next_pick(list<int>* A, list<int>* B);
    
    int get_element(list<int>* _list, int index);

public:
    Greedy_2regret_cycle(string file);
    Greedy_2regret_cycle(string file, int a);
    ~Greedy_2regret_cycle();
};





int Greedy_2regret_cycle::calc_regret(vector<int> costs, int k)
{
    int ret = 0;

    for(int i=1; i < k && i < costs.size(); i++)
    {
        ret += abs(costs[0] - costs[i]);
    }

    return ret;
}

int  Greedy_2regret_cycle::get_element(list<int>* _list, int index)
{
    list<int>::iterator it = _list->begin();
    advance(it,index);
    return *it;
}



int Greedy_2regret_cycle::next_pick(list<int>* A, list<int>* B)
{
    
    int selected = -1;
    pair<int,int> selected_p;
    
    int max_regret = -1;
    
    for(int i = 0; i < coords.size(); i++)
    {
        if(taken[i])
            continue;
        
        vector<int> costs;
        int min_cost = INT_MAX;
        pair<int,int> min_p;

        for(int j = 0; j < A->size(); j++)
        {
            pair<int,int> new_p;
            new_p.first = get_element(A, j);
            
            if(j+1 == A->size())
                new_p.second = get_element(A, 0);//cycle[0];
            else
                new_p.second = get_element(A, j+1);//cycle[j+1]
            
            int new_cost = matrix[new_p.first][i] + matrix[new_p.second][i] - matrix[new_p.first][new_p.second];
            costs.push_back(new_cost);
            

            if(min_cost == INT_MAX || new_cost < min_cost)
            {
                min_cost = new_cost;
                min_p = new_p;
            }
            
        }
        std::sort(costs.begin(), costs.end());
        int new_regret = calc_regret(costs,2) - costs.front()/3.0;

        if(max_regret == -1 || max_regret < new_regret)
        {
            max_regret = new_regret;
            selected_p = min_p;
            selected = i;
        }

        // gdy zale sa rowne sprawdzamy koszty (ta sytuacja dzieje sie zawsze na poczatku)
        else if(max_regret == new_regret)
        {
            int old_cost = matrix[selected_p.first][selected] + matrix[selected_p.second][selected] - matrix[selected_p.first][selected_p.second];
            int new_cost = matrix[min_p.first][i] + matrix[min_p.second][i] - matrix[min_p.first][min_p.second];
            if(old_cost > new_cost)
            {
                max_regret = new_regret;
                selected_p = min_p;
                selected = i;
            }
        }

    }

    // list<int>::iterator it = A->begin();
    // advance(it,selected_p.second);
    list<int>::iterator it = std::find(A->begin(), A->end(), selected_p.second);
    A->insert(it,selected);
    taken[selected] = true;
    
    // zwracana jest wartosc o jaka nowy cykl jest dluzszy
    return matrix[selected_p.first][selected] + matrix[selected][selected_p.second] - matrix[selected_p.first][selected_p.second];

}

void Greedy_2regret_cycle::run()
{
    list<int>* A = &A_cycle;
    list<int>* B = &B_cycle;

    int* a_len = &A_length;
    int* b_len = &B_length;

    
    while(A->size() + B->size() < coords.size())
    {
        
        *a_len += next_pick(A,B);
        swap(A,B);
        swap(a_len,b_len);
            
        // printf("\rPozostalo: %d   ", coords.size() - (A->size() + B->size()));
        // cout<< A->size() + B->size() << endl;
    }
    // cout << endl;
}

Greedy_2regret_cycle::Greedy_2regret_cycle(string file): Cycle_abstract(file)
{run();}

Greedy_2regret_cycle::Greedy_2regret_cycle(string file, int a) : Cycle_abstract(file, a)
{run();}

Greedy_2regret_cycle::~Greedy_2regret_cycle()
{}

#endif