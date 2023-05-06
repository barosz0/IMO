#include <iostream>
#include "Greedy_2regret_cycle.hpp"
// #include "Local_search_steepest.hpp"
// #include "Local_search_greedy.hpp"
#include "Memory_search.hpp"
#include "Random_cycle.hpp"
#include <chrono>
#include "Multiple_start_search.hpp"
#include "Local_search_steepest.hpp"
#include "Iterated_local_search_small.hpp"
#include "Iterated_local_search_big.hpp"



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
template <class Cycle>
void make_data(string filename, int a)
{
    Cycle cycle(filename, a);
    save_coords_list(cycle.get_cycle_coords(cycle.A_cycle), cycle.get_cycle_coords(cycle.B_cycle), "output/before.data");

    cout << "Przed" << cycle.get_length() << endl;

    // cout << "-------Local Search Greedy Edge Neighborhood-------" << endl;
    // Local_search_steepest LSSE(&cycle, EDGE_NEIGHBORHOOD);
    // cout << "Po: " << LSSE.get_length() << endl;
    // save_coords_list(LSSE.get_A_coords(), LSSE.get_B_coords(), "output/after_LSS_E.data");

    cout << "-------Local Memory Search-------" << endl;
    Memory_search MS(&cycle);
    cout << "Po: " << MS.get_length() << endl;
    save_coords_list(MS.get_A_coords(), MS.get_B_coords(), "output/after_MS.data");
}

template < class Cycle, class LS >
void calc_data2(string filename, int times = 1, string name = "")
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
    int min_len = 99999999;
    int max_len = -1;
    int len;

    cout << "-------" << name << "-------" << endl;
    
    avg_len = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 100; i++)
    {
        for(int t = 0; t < times; t++)
        {
            len = LS(&cycles[i]).get_length();
            avg_len += len;
            min_len = min(min_len,len);
            max_len = max(max_len,len);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    avg_len /= 100*times;
    time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    time /= 100*times;

    cout << "Sredni czas:\t\t" << time << " (micro)s"<< endl;
    cout << "Srednia dlugosc:\t" << avg_len << endl;
    cout << "Srednia poprawa:\t" << avg_start - avg_len << endl;
    cout << "Minimalna dlugosc:\t" << min_len << endl;
    cout << "Maksymalna dlugosc:\t" << max_len << endl;
}

int main()
{
    
    // make_data<Random_cycle>("Data/kroA200.tsp", 20);
    // Greedy_2regret_cycle cycle("Data/kroB100.tsp");
    // Memory_search ms(&cycle);
    
    // calc_data<Random_cycle, Local_search_steepest>("Data/kroB200.tsp",1,"Local Search Greedy Edge Neighborhood");
    // calc_data2<Random_cycle, Memory_search>("Data/kroB200.tsp", 1, "Memory_search");
    // int a = 0, b = 0;
    // for(int i = 0; i < 300; i++)
    // {
    //     // Random_cycle cycle("Data/kroA200.tsp");
    //     Random_cycle cycle("Data/kroA100.tsp", 0, i);
    //     printf("i:%d\t %d\n", i, cycle.get_length());
    //     Memory_search MS(&cycle);
    //     Local_search_steepest LSS(&cycle, EDGE_NEIGHBORHOOD);
        
    //     if(MS.get_length() > LSS.get_length()) a++;
    //     else b++;

    //     printf("MS: %d\t LSS:%d\n", a, b);
    //     // printf("MS: %d\t LSS:%d\n", MS.get_length(), LSS.get_length());
    // }
  
    // Random_cycle cycle("Data/kroA200.tsp", 0, 11); // 117
    // Memory_search MS(&cycle);
    
    printf("Start\n");
    // Multiple_start_search<Memory_search,Random_cycle> LS("Data/kroA200.tsp", 100);
    // Multiple_start_search<Memory_search,Greedy_2regret_cycle> LS("Data/kroA200.tsp", 100);
    // Random_cycle cycle("Data/kroA200.tsp");
    Greedy_2regret_cycle cycle("Data/kroA200.tsp");
    // Iterated_local_search_small LS(&cycle);
    Iterated_local_search_big LS(&cycle);

    // Memory_search MS(&cycle);
    printf("Final: %d\n", LS.get_length());
    printf("all OK\n");



    return 0;
}

// gcc .\Lab4.cpp *.hpp -lstdc++ -g