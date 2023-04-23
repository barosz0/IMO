#ifndef MEMORY_SEARCH
#define MEMORY_SEARCH

#include "Local_search_abstract.hpp"
#include <queue>
#include <stack>

#define INNER_CHANGE 1123
#define OUTER_CHANGE 1124

struct move_data
{
    int type;
    int a;
    int b;
    int diff;

    // only in INNER_CHANGE
    int next_a;
    int next_b;
    int prev_a;
    int prev_b;
    int which_cycle;

    bool operator<(const move_data& rhs) const
    {
        return diff < rhs.diff;
    }
};

int getIndex(vector<int> v, int K)
{
    auto it = find(v.begin(), v.end(), K);
  
    if (it != v.end()) 
    {
        int index = it - v.begin();
       return index;
    }
    else {
        return -1;
    }
}

class Memory_search: public Local_search_abstract
{
private:
    /* data */
    void run() override;
    void generate_all_moves();
    priority_queue<move_data> moves; 
    stack<move_data> moves_beta; // ruchy nie mozliwe w tym momecie ale mozliwe pozniej
    int check_posible(move_data move);
    int check_order(int index, int value,vector<int>& cycle);
    void make_move(move_data move);

    void generate_moves_outer(int index_a, vector<int>& a_cycle, vector<int>& b_cycle);
public:
    Memory_search(Cycle_abstract *cycle);
    ~Memory_search();
};

void Memory_search::generate_all_moves()
{
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

            if( 0 > new_len)
            {
                move_data m;
                m.a = A_cycle[a];
                m.b = B_cycle[b];
                m.diff = new_len;
                m.type = OUTER_CHANGE;
                moves.push(m);
            }

        }
    }

    for(int cycle_index = 0; cycle_index<2;cycle_index++){
        vector<int>& cycle = (cycle_index==0) ? A_cycle : B_cycle;
        for(int a = 0; a < cycle.size(); a++)
        {
            for(int b = a+1; b < cycle.size(); b++)
            {

                if (a == b-1) continue;

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


                if(0 > new_len)
                {
                    move_data m;
                    m.a = cycle[a];
                    m.next_a = cycle[a+1];

                    m.b = cycle[b];
                    if(b + 1 < cycle.size())
                        m.next_b = cycle[b+1];
                    else
                        m.next_b = cycle[0];
                    m.diff = new_len;
                    m.type = INNER_CHANGE;
                    m.which_cycle = cycle_index;
                    moves.push(m);
                }

            }
        }
    }
    // auto A_reversed = A_cycle;
    // reverse(A_reversed.begin(), A_reversed.end());
    // auto B_reversed = B_cycle;
    // reverse(B_reversed.begin(), B_reversed.end());

    // for(int cycle_index = 0; cycle_index<2;cycle_index++){
    //     vector<int>& cycle = (cycle_index==0) ? A_reversed : B_reversed;
    //     for(int a = 0; a < cycle.size(); a++)
    //     {
    //         for(int b = a+1; b < cycle.size(); b++)
    //         {

    //             if (a == b-1) continue;

    //             int new_len = 0;

    //             new_len -= matrix[cycle[a]][cycle[a+1]];

    //             if(b + 1 < cycle.size())
    //                 new_len -= matrix[cycle[b]][cycle[b+1]];
    //             else // ostatnia krawedz
    //                 new_len -= matrix[cycle[b]][cycle[0]];

    //             new_len += matrix[cycle[a]][cycle[b]];
    //             if(b + 1 < cycle.size())
    //                 new_len += matrix[cycle[a+1]][cycle[b+1]];
    //             else
    //                 new_len += matrix[cycle[a+1]][cycle[0]];


    //             if(0 > new_len)
    //             {
    //                 move_data m;
    //                 m.a = cycle[a];
    //                 m.next_a = cycle[a+1];

    //                 m.b = cycle[b];
    //                 if(b + 1 < cycle.size())
    //                     m.next_b = cycle[b+1];
    //                 else
    //                     m.next_b = cycle[0];
    //                 m.diff = new_len;
    //                 m.type = INNER_CHANGE;
    //                 m.which_cycle = cycle_index;
    //                 moves.push(m);
    //             }

    //         }
    //     }
    // }
}

int Memory_search::check_order(int index, int value,vector<int>& cycle)
{
    int index_next = (index+1 < cycle.size()) ? index+1 : 0;
    int index_prev = (index == 0) ? cycle.size() -1: index-1;

    if(cycle[index_next] == value)
        return 0;
    
    if(cycle[index_prev] == value)
        return 1;
    
    return -1;

}

// 1 - ruch morzliwy
// 0 - ruch do usuniecia
// -1 - niemozliwy ale nie usuwac
int Memory_search::check_posible(move_data move)
{
    if(move.type == OUTER_CHANGE)
    {
        if(std::find(A_cycle.begin(), A_cycle.end(), move.a) == A_cycle.end() || 
        std::find(B_cycle.begin(), B_cycle.end(), move.b) == B_cycle.end())
        {
            return 0;
        }
        return 1;
    }

    if(move.type == INNER_CHANGE)
    {
        vector<int>& cycle = (move.which_cycle==0) ? A_cycle : B_cycle;

        int a_index = getIndex(cycle, move.a);
        int b_index = getIndex(cycle, move.b);

        if(a_index == -1 || b_index == -1)
            return 0;
        
        bool a_order = check_order(a_index, move.next_a, cycle);
        bool b_order = check_order(b_index, move.next_b, cycle);

        if(a_order == -1 || b_order == -1)
            return 0;
        
        if(a_order == b_order)
            return 1;
        else
            return -1;
    }
    return 0;
}

void Memory_search::generate_moves_outer(int index_a, vector<int>& a_cycle, vector<int>& b_cycle)
{
    
    pair<int,int> neigh_A = get_adjacent_vertex(a_cycle, index_a);
    for(int b=0; b < b_cycle.size();b++)
        {


            pair<int,int> neigh_B = get_adjacent_vertex(b_cycle, b);
            int new_len = 0;


            new_len -= matrix[neigh_A.first][a_cycle[index_a]] + matrix[a_cycle[index_a]][neigh_A.second];
            new_len -= matrix[neigh_B.first][b_cycle[b]] + matrix[b_cycle[b]][neigh_B.second];


            new_len += matrix[neigh_A.first][b_cycle[b]] + matrix[b_cycle[b]][neigh_A.second];
            new_len += matrix[neigh_B.first][a_cycle[index_a]] + matrix[a_cycle[index_a]][neigh_B.second];

            if( 0 > new_len)
            {
                move_data m;
                m.a = a_cycle[index_a];
                m.b = b_cycle[b];
                m.diff = new_len;
                m.type = OUTER_CHANGE;
                moves.push(m);
            }

        }
}


void Memory_search::make_move(move_data move)
{
    if(move.type == OUTER_CHANGE)
    {
        int index_a = getIndex(A_cycle,move.a);
        int index_b = getIndex(B_cycle,move.b);

        swap(A_cycle[index_a],B_cycle[index_b]);
        printf("Swap %d %d\n",index_a, index_b);

        //TODO dodaj nowe ruchy
        generate_moves_outer(index_a,A_cycle,B_cycle);
        // generate_moves_outer(index_b,B_cycle,A_cycle);

        
    }

    if(move.type == INNER_CHANGE)
    {
        vector<int>& cycle = (move.which_cycle==0) ? A_cycle : B_cycle;
        int index_a = getIndex(cycle,move.a);
        int index_b = getIndex(cycle,move.b);

        if (index_a > index_b)
            swap(index_a,index_b);

        printf("reverse %d %d\n", index_a+1, index_b+1);
        reverse(cycle.begin()+index_a+1,cycle.begin()+index_b+1);
        //TODO dodaj nowe ruchy

        

        

    }

}

void Memory_search::run()
{
    move_data move;
    while (!moves.empty())
    {
        
        move = moves.top();
        moves.pop();
        
        int status = check_posible(move);
        if (status == 1)
        {
            make_move(move);
            while (!moves_beta.empty())
            {
                moves.push(moves_beta.top());
                moves_beta.pop();
            }
        }

        if (status == -1)
        {
            moves_beta.push(move);
        }
    }
    
}

Memory_search::Memory_search(Cycle_abstract *cycle) : Local_search_abstract(cycle)
{
    printf("Start: %d\n",get_length());
    generate_all_moves();
    run();
    printf("Koniec: %d\n",get_length());
}

Memory_search::~Memory_search()
{
}

#endif