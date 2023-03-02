#include <iostream>
#include "parser.hpp"
#include "Greedy_cycle.hpp"

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

int main()
{
    // parser p("Data/kroA100.tsp");
    // parser p("Data/kroB100.tsp");
    cout << "---------------start---------------" << endl;
    Greedy_cycle cycle("Data/kroB100.tsp");
    cout << "---------------end---------------" << endl;
    cout << "Dlugosc :" << cycle.cycle_length << endl;

    cout << "Cykl A:" << endl;
    showlist(cycle.A_cycle);

    cout << "Cykl B:" << endl;
    showlist(cycle.B_cycle);


    // print_2d_vector(p.matrix);

}

