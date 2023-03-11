#include <iostream>
#include "parser.hpp"
#include "Greedy_cycle.hpp"
#include "Greedy_2regret_cycle.hpp"
#include "nearest_neighbor.hpp"

void greedy_2regret_cycle_print_data(string filename)
{
    int min, max, avg=0;
    cout << "--------------------------------" << endl; 
    cout << "Greedy 2regret cycle Algorithm" << endl;
    cout << "Dla pliku: " << filename << endl; 
    for(int i = 0; i < 100;i++)
    {
        Greedy_2regret_cycle cycle(filename,i);
        int full_length = cycle.A_length + cycle.B_length;

        if(i==0)
        {
            min = full_length;
            max = full_length;
        }
        else
        {
            if(min > full_length) min = full_length;
            if(max < full_length) max = full_length;
        }
        avg += full_length;
    }
    avg/=100;

    cout << "Minimum:\t" << min << endl;
    cout << "Maksimum:\t" << max << endl;
    cout << "srednia:\t" << avg << endl;
}

void nearest_neighbor_data(string filename)
{
    int min, max, avg=0;
    cout << "--------------------------------" << endl; 
    cout << "Greedy  cycle Algorithm" << endl;
    cout << "Dla pliku: " << filename << endl; 
    for(int i = 0; i < 100;i++)
    {
        Nearest_neighbor cycle(filename,i);
        int full_length = cycle.A_length + cycle.B_length;
        if(i==0)
        {
            min = full_length;
            max = full_length;
        }
        else
        {
            if(min > full_length) min = full_length;
            if(max < full_length) max = full_length;
        }
        avg += full_length;
    }
    avg/=100;

    cout << "Minimum:\t" << min << endl;
    cout << "Maksimum:\t" << max << endl;
    cout << "srednia:\t" << avg << endl;
}

void greedy_cycle_print_data(string filename)
{
    int min, max, avg=0;
    cout << "--------------------------------" << endl; 
    cout << "Greedy  cycle Algorithm" << endl;
    cout << "Dla pliku: " << filename << endl; 
    for(int i = 0; i < 100;i++)
    {
        Greedy_cycle cycle(filename,i);
        int full_length = cycle.A_length + cycle.B_length;
        if(i==0)
        {
            min = full_length;
            max = full_length;
        }
        else
        {
            if(min > full_length) min = full_length;
            if(max < full_length) max = full_length;
        }
        avg += full_length;
    }
    avg/=100;

    cout << "Minimum:\t" << min << endl;
    cout << "Maksimum:\t" << max << endl;
    cout << "srednia:\t" << avg << endl;
}

int main()
{
    string filename = "data/kroA100.tsp";
    greedy_2regret_cycle_print_data(filename);
    greedy_cycle_print_data(filename);
    nearest_neighbor_data(filename);
    return 0;

}

