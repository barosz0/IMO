#include <iostream>
#include "Greedy_2regret_cycle.hpp"
#include "Local_search_steepest.hpp"
#include "Local_search_greedy.hpp"


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

int main()
{
    
    Greedy_2regret_cycle cycle("Data/kroB100.tsp", 14);
    cout << "Przed: " <<cycle.A_length + cycle.B_length << endl;
    Local_search_greedy LS(&cycle, VERTEX_NEIGHBORHOOD);
    cout << "Po: " << LS.get_length() << endl;
    save_coords_list(cycle.get_cycle_coords(cycle.A_cycle), cycle.get_cycle_coords(cycle.B_cycle), "output/before.data");
    save_coords_list(LS.get_A_coords(), LS.get_B_coords(), "output/after.data");

    cout << "Koniec!!!!!!!!!" << endl;
    
    return 0;
}
