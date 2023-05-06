#ifndef MULTIPLE_START_SEARCH
#define MULTIPLE_START_SEARCH

#include "Local_search_abstract.hpp"

template <class LS_, class cycle_>
class Multiple_start_search: public Local_search_abstract
{
private:
    /* data */
    string filename;
    int iterations;

    virtual void run();
public:
    Multiple_start_search(string filename_, int iterations_);
    Multiple_start_search(string filename_);
    ~Multiple_start_search();
};

template <class LS_, class cycle_>
void Multiple_start_search<LS_,cycle_>::run()
{
    for(int i = 0; i<iterations; i++)
    {
        cycle_ &cycle = * new cycle_(filename, 0, i);
        LS_ LS(&cycle);

        if(A_cycle.size() + B_cycle.size() == 0 || LS.get_length() < get_length())
        {
            A_cycle = LS.get_A_cycle();
            B_cycle = LS.get_B_cycle();
            orginal_cycle = &cycle;
            recalculate_length();
        }
        printf("i:%d\tlen:%d\n", i, get_length());
    }
}

template <class LS_, class cycle_>
Multiple_start_search<LS_,cycle_>::Multiple_start_search(string filename_, int iterations_)
{
    parser p(filename_);
    matrix = p.matrix;

    filename = filename_;
    iterations = iterations_;

    run();

    printf("%d\n", get_length());
}

template <class LS_, class cycle_>
Multiple_start_search<LS_,cycle_>::Multiple_start_search(string filename_) : Multiple_start_search(filename_,100) 
{
}

template <class LS_, class cycle_>
Multiple_start_search<LS_,cycle_>::~Multiple_start_search()
{
}

#endif