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
#include "Hybrid.hpp"


int times_test_max = 10;
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
void calc_data(string filename, int times = 1, string name = "")
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

    cout<< "                                                              \r";
    cout << "Sredni czas:\t\t" << time << " (micro)s"<< endl;
    cout << "Srednia dlugosc:\t" << avg_len << endl;
    cout << "Srednia poprawa:\t" << avg_start - avg_len << endl;
    cout << "Minimalna dlugosc:\t" << min_len << endl;
    cout << "Maksymalna dlugosc:\t" << max_len << endl;
}

long long do_MSLS()
{
    
    long long time;
    int avg_len;
    int min_len = 99999999;
    int max_len = -1;
    int len;

    cout << "-------" << "MULTIPLE START LOCAL SEARCH" << "-------" << endl;
    
    avg_len = 0;
    int avg_iter = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for(int t = 0; t < times_test_max; t++)
    {
        auto m = Multiple_start_search<Memory_search,Random_cycle>("Data/kroA200.tsp", 100);
        len = m.get_length();
        avg_len += len;
        min_len = min(min_len,len);
        max_len = max(max_len,len);
        avg_iter += m.get_moves();
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    avg_len /= times_test_max;
    time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    time /= times_test_max;
    avg_iter /= times_test_max;

    cout<< "                                                              \r";
    cout << "Sredni czas:\t\t" << time << " (micro)s"<< endl;
    cout << "Srednia dlugosc:\t" << avg_len << endl;
    cout << "Minimalna dlugosc:\t" << min_len << endl;
    cout << "Maksymalna dlugosc:\t" << max_len << endl;
    cout << "Ilosc iteracji:\t" << avg_iter << endl;

    return time;
}

void do_ILS1(int time)
{
    int avg_len;
    int min_len = 99999999;
    int max_len = -1;
    int len;

    cout << "-------" << "ITERATED LOCAL SEARCH 1" << "-------" << endl;
    
    int avg_iter = 0;
    avg_len = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for(int t = 0; t < times_test_max; t++)
    {
        Random_cycle cycle("Data/kroA200.tsp");
        auto m = Iterated_local_search_small(&cycle,time);
        len = m.get_length();
        avg_len += len;
        min_len = min(min_len,len);
        max_len = max(max_len,len);
        avg_iter += m.get_moves();
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    avg_len /= times_test_max;
    time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    time /= times_test_max;
    avg_iter /= times_test_max;

    cout<< "                                                              \r";
    cout << "Sredni czas:\t\t" << time << " (micro)s"<< endl;
    cout << "Srednia dlugosc:\t" << avg_len << endl;
    cout << "Minimalna dlugosc:\t" << min_len << endl;
    cout << "Maksymalna dlugosc:\t" << max_len << endl;
    cout << "Ilosc iteracji:\t" << avg_iter << endl;
}

void do_ILS2(int time, bool do_local_search = false)
{
    int avg_len;
    int min_len = 99999999;
    int max_len = -1;
    int len;
    string tekst;
    if(do_local_search)
        tekst = "(with local search)";
    else
        tekst = "(without local search)";

    cout << "-------" << "ITERATED LOCAL SEARCH 2 " << tekst << "-------" << endl;
    
    int avg_iter = 0;
    avg_len = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for(int t = 0; t < times_test_max; t++)
    {
        Greedy_2regret_cycle cycle("Data/kroA200.tsp");
        auto m = Iterated_local_search_big(&cycle,time, do_local_search);
        len = m.get_length();
        avg_len += len;
        min_len = min(min_len,len);
        max_len = max(max_len,len);
        avg_iter += m.get_moves();
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    avg_len /= times_test_max;
    time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    time /= times_test_max;
    avg_iter /= times_test_max;

    cout << "Sredni czas:\t\t" << time << " (micro)s"<< endl;
    cout << "Srednia dlugosc:\t" << avg_len << endl;
    cout << "Minimalna dlugosc:\t" << min_len << endl;
    cout << "Maksymalna dlugosc:\t" << max_len << endl;
    cout << "Ilosc iteracji:\t" << avg_iter << endl;
}

void do_Hybrid(int time, bool do_local_search = false)
{
    int avg_len;
    int min_len = 99999999;
    int max_len = -1;
    int len;
    string tekst;
    if(do_local_search)
        tekst = "(with local search)";
    else
        tekst = "(without local search)";

    cout << "-------" << "Hybrid Evolutionary " << tekst << "-------" << endl;
    
    int avg_iter = 0;
    avg_len = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for(int t = 0; t < times_test_max; t++)
    {
        auto m = Hybrid("Data/kroA200.tsp", time);
        len = m.get_length();
        avg_len += len;
        min_len = min(min_len,len);
        max_len = max(max_len,len);
        avg_iter += m.get_moves();
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    avg_len /= times_test_max;
    time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    time /= times_test_max;
    avg_iter /= times_test_max;

    cout << "Sredni czas:\t\t" << time << " (micro)s"<< endl;
    cout << "Srednia dlugosc:\t" << avg_len << endl;
    cout << "Minimalna dlugosc:\t" << min_len << endl;
    cout << "Maksymalna dlugosc:\t" << max_len << endl;
    cout << "Ilosc iteracji:\t" << avg_iter << endl;
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
    // Greedy_2regret_cycle cycle("Data/kroA200.tsp");
    // Iterated_local_search_small LS(&cycle);
    // Iterated_local_search_big LS(&cycle);

    // Memory_search MS(&cycle);
    // printf("Final: %d\n", LS.get_length());
    // do_MSLS();
    // Hybrid("Data/kroA200.tsp",26133734);
    int time = do_MSLS(); //  36133734; // 
    do_ILS1(time);
    do_ILS2(time, false);
    do_ILS2(time, true);
    do_Hybrid(time, false);
    do_Hybrid(time, true);
    printf("all OK\n");



    return 0;
}

// gcc .\Lab4.cpp *.hpp -lstdc++ -g