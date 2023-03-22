#include <iostream>
#include "parser.hpp"
#include "Greedy_cycle.hpp"
#include "Greedy_2regret_cycle.hpp"
#include "nearest_neighbor.hpp"

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

void show_coords_list(vector<pair<double, double>> cycle_coords )
{
    list<int>::iterator it;
    cout << '[';

    for(int j = 0; j < cycle_coords.size(); j++){
        cout << "(" << cycle_coords[j].first <<","<< cycle_coords[j].second << ")";
        if (j != cycle_coords.size() -1 )
            cout <<',';
    }

    cout << "]\n";
   

}

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

void greedy_cycle_run()
{
    cout << "Greedy cycle Algorithm" << endl;
    cout << "---------------start---------------" << endl;
    Greedy_cycle cycle("Data/kroB100.tsp");
    cout << "---------------end---------------" << endl;

    cout << "Cykl A:" << endl;
    // showlist(cycle.A_cycle);
    cout << "Dlugosc :" << cycle.A_length << endl;
    cout << "Kolejne wspolrzedne cyklu A" << endl;
    // show_coords_list(cycle.get_cycle_coords(cycle.A_cycle));


    cout << "Cykl B:" << endl;
    // showlist(cycle.B_cycle);
    cout << "Dlugosc :" << cycle.B_length << endl;
    cout << "Kolejne wspolrzedne cyklu B" << endl;
    // show_coords_list(cycle.get_cycle_coords(cycle.B_cycle));

    save_coords_list(cycle.get_cycle_coords(cycle.A_cycle),cycle.get_cycle_coords(cycle.B_cycle), "output/greedy_cycle.data");
}

void greedy_2regret_cycle_run()
{
    cout << "Greedy 2regret cycle Algorithm" << endl;
    cout << "---------------start---------------" << endl;
    Greedy_2regret_cycle cycle("Data/kroB100.tsp",90);
    cout << "---------------end---------------" << endl;

    cout << "Cykl A:" << endl;
    // showlist(cycle.A_cycle);
    cout << "Dlugosc :" << cycle.A_length << endl;
    cout << "Kolejne wspolrzedne cyklu A" << endl;
    // show_coords_list(cycle.get_cycle_coords(cycle.A_cycle));

    cout << "Cykl B:" << endl;
    // showlist(cycle.B_cycle);
    cout << "Dlugosc :" << cycle.B_length << endl;
    cout << "Kolejne wspolrzedne cyklu B" << endl;
    // show_coords_list(cycle.get_cycle_coords(cycle.B_cycle));

    save_coords_list(cycle.get_cycle_coords(cycle.A_cycle),cycle.get_cycle_coords(cycle.B_cycle), "output/greedy_2regret_cycle.data");
}

void Nearest_neighbor_run()
{
    cout << "Nearest neighbor Algorithm" << endl;
    cout << "---------------start---------------" << endl;
    Nearest_neighbor cycle("Data/kroB100.tsp");
    cout << "---------------end---------------" << endl;

    cout << "Cykl A:" << endl;
    // showlist(cycle.A_cycle);
    cout << "Dlugosc :" << cycle.A_length << endl;
    cout << "Kolejne wspolrzedne cyklu A" << endl;
    // show_coords_list(cycle.get_cycle_coords(cycle.A_cycle));

    cout << "Cykl B:" << endl;
    // showlist(cycle.B_cycle);
    cout << "Dlugosc :" << cycle.B_length << endl;
    cout << "Kolejne wspolrzedne cyklu B" << endl;
    // show_coords_list(cycle.get_cycle_coords(cycle.B_cycle));

    save_coords_list(cycle.get_cycle_coords(cycle.A_cycle),cycle.get_cycle_coords(cycle.B_cycle), "output/nearest_neighbor.data");
}

int main()
{

    // parser p("Data/kroA100.tsp");
    // parser p("Data/kroB100.tsp");

    greedy_cycle_run();
    
    greedy_2regret_cycle_run();

    Nearest_neighbor_run();
    


    // print_2d_vector(p.matrix);

}

