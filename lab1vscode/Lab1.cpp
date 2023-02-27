#include <iostream>
#include "parser.hpp"

void print_2d_vector(const std::vector<std::vector<int>>& vec) {
    for (const auto& row : vec) {
        for (const auto& elem : row) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
}

int main()
{
    parser p("Data/kroA100.tsp");
    // parser p("Data/kroB100.tsp");

    // print_2d_vector(p.matrix);

}

