#include <iostream>
#include "parser.hpp"
#include "Greedy_cycle.hpp"
#include "Greedy_2regret_cycle.hpp"

void print_2d_vector(const std::vector<std::vector<int>>& vec) {
    for (const auto& row : vec) {
        for (const auto& elem : row) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
}

void showlist(list<int> g)
{
    list<int>::iterator it;
    for (it = g.begin(); it != g.end(); ++it)
        cout << '\t' << *it;
    cout << '\n';
}

void greedy_cycle_run()
{
    cout << "Greedy cycle Algorithm" << endl;
    cout << "---------------start---------------" << endl;
    Greedy_cycle cycle("Data/kroB100.tsp");
    cout << "---------------end---------------" << endl;

    cout << "Cykl A:" << endl;
    showlist(cycle.A_cycle);
    cout << "Dlugosc :" << cycle.A_length << endl;

    cout << "Cykl B:" << endl;
    showlist(cycle.B_cycle);
    cout << "Dlugosc :" << cycle.B_length << endl;
}

void greedy_2regret_cycle_run()
{
    cout << "Greedy 2regret cycle Algorithm" << endl;
    cout << "---------------start---------------" << endl;
    Greedy_2regret_cycle cycle("Data/kroB100.tsp");
    cout << "---------------end---------------" << endl;

    cout << "Cykl A:" << endl;
    showlist(cycle.A_cycle);
    cout << "Dlugosc :" << cycle.A_length << endl;

    cout << "Cykl B:" << endl;
    showlist(cycle.B_cycle);
    cout << "Dlugosc :" << cycle.B_length << endl;
}

int main()
{

    // parser p("Data/kroA100.tsp");
    // parser p("Data/kroB100.tsp");

    greedy_cycle_run();
    
    greedy_2regret_cycle_run();
    


    // print_2d_vector(p.matrix);

}

