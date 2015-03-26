#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "datatype.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    
    std::string fpath("data/");
    if(argc > 1) {
        fpath += argv[1];
    } else {
        fpath += "debug";
    }

    int job, machine;
    std::string fname;
    std::ifstream fin;
    fin.open(fpath.c_str());

    fin >> job >> machine >> fname;

    Schedule schedule(job, machine);
    Solution solution;

    for (int i = 0; i < machine; ++i)
    {
        for (int j = 0; j < job; ++j)
        {
            fin >> schedule.getMatrix()[i][j];
        }
    }
    schedule.Print();
    

    const int TIMES_TO_HYBRID = 5000;
    const int SOLUTION_TO_GEN = 50;
    const int NEIGHBORHOOD_SIZE = 10;
    const int TABULIST_SIZE = 10;
    for (int sol = 0; sol < SOLUTION_TO_GEN; ++sol)
    {
        Tabulist tabulist(TABULIST_SIZE);
        Schedule new_schedule(schedule);
        Neighbor nb;
        for (int tfind = 0; tfind < TIMES_TO_HYBRID; ++tfind)
        {
            nb = new_schedule.FindNeighbor(NEIGHBORHOOD_SIZE, tabulist);
            new_schedule.Visit(nb);
            tabulist.Push(nb);
        }
        solution.Push(nb.getValue());
    }

    solution.Print();

    return 0;
}