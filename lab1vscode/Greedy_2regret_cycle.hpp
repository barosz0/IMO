#include "parser.hpp"
#include <cstdlib>
#include <list>

class Greedy_2regret_cycle
{
private:
    vector<vector<int>> matrix;
    vector<pair<double, double>> coords; 
    vector<bool> taken;
    
    
    void run();
    int next_pick(list<int>* A, list<int>* B);
    int calc_regret(list<int>* B, int element);

    void choose_random_starting(int &a,int &b);
    int get_element(list<int>* _list, int index);

public:
    int cycle_length;
    list<int> A_cycle;
    list<int> B_cycle;
    int A_length;
    int B_length;

    Greedy_2regret_cycle(string file);
    ~Greedy_2regret_cycle();
};

int Greedy_2regret_cycle::calc_regret(list<int>* B, int element)
{
    int selected = -1;
    int min_cost = -1;

    int new_cost;
    for(int j = 0; j < B->size(); j++)
    {
        pair<int,int> new_p;
        new_p.first = get_element(B, j);
        if(j+1 == B->size())
            new_p.second = get_element(B, 0);//cycle[0];
        else
            new_p.second = get_element(B, j+1);//cycle[j+1]

        for(int i = 0; i < coords.size(); i++)
        {
            if(!taken[i] && i!= element)
            {
                new_cost = matrix[new_p.first][i] + matrix[new_p.second][i] - matrix[new_p.first][new_p.second];

                if(selected == -1)
                {
                    selected = i;
                    min_cost = new_cost;
                    continue;
                }
                
                if(new_cost < min_cost)
                {
                    min_cost = new_cost;
                    selected = i;
                }
            }    
        }
    }

    return min_cost;
}


int  Greedy_2regret_cycle::get_element(list<int>* _list, int index)
{
    list<int>::iterator it = _list->begin();
    advance(it,index);
    return *it;
}

void Greedy_2regret_cycle::choose_random_starting(int &a, int &b)
{
    srand((unsigned) time(NULL));
    a = rand() % coords.size();

    b = 0;
    for(int i = 1; i < coords.size(); i++)
    {
        if(matrix[a][b] < matrix[a][i])
        {
            b = i;
        }
    }
}



int Greedy_2regret_cycle::next_pick(list<int>* A, list<int>* B)
{
    
    pair<int,int> min_p;
    int selected = -1;
    int min_cost = -1;

    int new_cost;
    for(int j = 0; j < A->size(); j++)
    {
        pair<int,int> new_p;
        new_p.first = get_element(A, j);
        
        if(j+1 == A->size())
            new_p.second = get_element(A, 0);//cycle[0];
        else
            new_p.second = get_element(A, j+1);//cycle[j+1]

        for(int i = 0; i < coords.size(); i++)
        {
            
            if(!taken[i])
            {
                new_cost = matrix[new_p.first][i] + matrix[new_p.second][i] - matrix[new_p.first][new_p.second] - calc_regret(B,i);

                if(selected == -1)
                {
                    selected = i;
                    min_cost = new_cost; 
                    min_p = new_p;
                    continue;
                }

                
                
                if(new_cost < min_cost)
                {
                    min_p = new_p;
                    min_cost = new_cost;
                    selected = i;
                }
            }
        }
    }

    list<int>::iterator it = A->begin();
    advance(it,min_p.second);
    A->insert(it,selected);
    taken[selected] = true;
    
    // zwracana jest wartosc o jaka nowy cykl jest dluzszy
    return matrix[min_p.first][selected] + matrix[selected][min_p.second] - matrix[min_p.first][min_p.second];

}

void Greedy_2regret_cycle::run()
{
    list<int>* A = &A_cycle;
    list<int>* B = &B_cycle;

    int* a_len = &A_length;
    int* b_len = &B_length;

    
    while(A->size() + B->size() < coords.size())
    {
        
        a_len += next_pick(A,B);
        swap(A,B);
        swap(a_len,b_len);
        // printf("\rPozostalo: %d   ", coords.size() - (A->size() + B->size()));
        // cout<< A->size() + B->size() << endl;
    }
    // cout << endl;
}

Greedy_2regret_cycle::Greedy_2regret_cycle(string file)
{
    parser p(file);

    matrix = p.matrix;
    coords = p.coords;

    int a,b;

    choose_random_starting(a,b);

    A_cycle = list<int>{a};
    A_length = 0;

    B_cycle = list<int>{b};
    B_length = 0;

    taken = vector<bool>(coords.size());
    taken[b] = taken[a] = true;

    run();
}

Greedy_2regret_cycle::~Greedy_2regret_cycle()
{
}
