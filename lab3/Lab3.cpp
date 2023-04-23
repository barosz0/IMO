#include <iostream>
#include "Greedy_2regret_cycle.hpp"
#include "Local_search_steepest.hpp"
#include "Local_search_greedy.hpp"
#include "Memory_search.hpp"
#include <chrono>


using namespace std;

void save_coords_list(vector<pair<double, double>> cycle_coords_A,vector<pair<double, double>> cycle_coords_B, string filename)
{
    ofstream output_file;
    output_file.open (filename);
    // myfile << "Writing this to a file.\n";

    for(int j = 0; j < cycle_coords_A.size(); j++){
        output_file <<  cycle_coords_A[j].first << " " << cycle_coords_A[j].second;
        if (j != cycle_coords_A.size() -1 )
            output_file <<';';
    }

    output_file << "\n"; 
    
    
    for(int j = 0; j < cycle_coords_B.size(); j++){
        output_file <<  cycle_coords_B[j].first << " " << cycle_coords_B[j].second;
        if (j != cycle_coords_B.size() -1 )
            output_file <<';';
    }


    output_file.close();
}

void make_data(string filename, int a)
{
    Greedy_2regret_cycle cycle(filename, a);
    save_coords_list(cycle.get_cycle_coords(cycle.A_cycle), cycle.get_cycle_coords(cycle.B_cycle), "output/before.data");

    cout << "Przed: " <<cycle.A_length + cycle.B_length << endl;
    cout << "-------Local Search Greedy Edge Neighborhood-------" << endl;
    Local_search_greedy LSGE(&cycle, EDGE_NEIGHBORHOOD);
    cout << "Po: " << LSGE.get_length() << endl;
    save_coords_list(LSGE.get_A_coords(), LSGE.get_B_coords(), "output/after_LSG_E.data");

    cout << "-------Local Search Greedy Vertex Neighborhood-------" << endl;
    Local_search_greedy LSGV(&cycle, VERTEX_NEIGHBORHOOD);
    cout << "Po: " << LSGV.get_length() << endl;
    save_coords_list(LSGV.get_A_coords(), LSGV.get_B_coords(), "output/after_LSG_V.data");

    cout << "-------Local Search Greedy Edge Neighborhood-------" << endl;
    Local_search_steepest LSSE(&cycle, EDGE_NEIGHBORHOOD);
    cout << "Po: " << LSSE.get_length() << endl;
    save_coords_list(LSSE.get_A_coords(), LSSE.get_B_coords(), "output/after_LSS_E.data");

    cout << "-------Local Search Steepest Vertex Neighborhood-------" << endl;
    Local_search_steepest LSSV(&cycle, VERTEX_NEIGHBORHOOD);
    cout << "Po: " << LSSV.get_length() << endl;
    save_coords_list(LSSV.get_A_coords(), LSSV.get_B_coords(), "output/after_LSS_V.data");
}

template < class Cycle, class LS >
void calc_data(string filename, int times = 1, string name = "", int type = VERTEX_NEIGHBORHOOD)
{
    vector<Cycle> cycles;
    int avg_start=0;
    for(int i = 0; i < 100; i++){
        cycles.push_back(Cycle(filename,i));
        avg_start += cycles[i].get_length();
    }
    avg_start/=100;
    
    long long time;
    int avg_len;

    cout << "-------" << name << "-------" << endl;
    
    avg_len = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 100; i++)
    {
        for(int t = 0; t < times; t++)
        {
            avg_len += LS(&cycles[i], type).get_length();
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    avg_len /= 100*times;
    time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    time /= 100*times;

    cout << "Sredni czas:\t\t" << time << " (micro)s"<< endl;
    cout << "Srednia dlugosc:\t" << avg_len << endl;
    cout << "Srednia poprawa:\t" << avg_start - avg_len << endl;
}

int main()
{
    
    // make_data("Data/kroB100.tsp", 14);
    // calc_data<Greedy_2regret_cycle, Local_search_steepest>("Data/kroB100.tsp",1,"Local Search Steepest Vertex Neighborhood");
    // calc_data<Greedy_2regret_cycle, Local_search_greedy>("Data/kroB100.tsp",1,"Local Search Greedy Vertex Neighborhood");
    Greedy_2regret_cycle cycle("Data/kroB100.tsp");
    Memory_search ms(&cycle);
    printf("all OK\n");
    return 0;
}

//gcc .\Lab3.cpp *.hpp -lstdc++ -g