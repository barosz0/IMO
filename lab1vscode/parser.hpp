#ifndef DATAPARSER
#define DATAPARSER


#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

class parser
{
private:
    void read_data(string file);
    void make_matrix();
public:
    vector<vector<int>> matrix;
    vector<pair<double, double>> coords; 
    parser(string file);
    ~parser();
};

parser::parser(string file)
{
    read_data(file);
    make_matrix();
}

parser::~parser()
{
}

void parser::read_data(string file_path)
{
    ifstream file_stream(file_path);
    if (!file_stream.is_open())
        throw std::runtime_error("Couldn't read! ");

    string line;

    for(int i=0;i<6;i++)
    {
        getline(file_stream, line);
        // cout << line << endl;
    }

    int i,a,b;

    while (file_stream >> i >> a >> b)
    {
        coords.push_back(make_pair(a,b));
    }
}

void parser::make_matrix()
{
    matrix = vector(coords.size(), vector<int> (coords.size(), 0));

    int dist;
    for(int i=0;i<coords.size();i++)
    {
        for(int j=i;j<coords.size();j++)
        {
            if(i==j)
            {
                matrix[i][j] = 0;
                matrix[j][i] = 0;
                continue;
            }
            dist = pow((double)coords[i].first - coords[j].first,2.0) + pow((double)coords[i].second - coords[j].second,2.0);
            dist = sqrt(dist);
            dist = round(dist);
            matrix[i][j] = dist;
            matrix[j][i] = dist;
        }
    }
}

#endif