#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "datatype.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    
    std::string finpath;
    std::string foutpath("output");

    if(argc >= 2) {
        finpath = argv[1];
        if(argc >= 3) {
            foutpath = argv[2];
        } else {
            foutpath += argv[1];
        }
    } else {
        finpath = "data/debug.txt";
        foutpath = "outputdata/debug.txt";
    }

    int job, machine;
    std::string fname;
    std::ifstream fin;
    std::ofstream fout;
    fin.open(finpath.c_str());
    fout.open(foutpath.c_str());

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
    schedule.Print(fout);
    

    const int TIMES_TO_HYBRID = 5000;
    const int SOLUTION_TO_GEN = 50;
    const int NEIGHBORHOOD_SIZE = 10;
    const int TABULIST_SIZE = 10;
    clock_t tTotal = 0;

    for (int sol = 0; sol < SOLUTION_TO_GEN; ++sol)
    {
        Tabulist tabulist(TABULIST_SIZE);
        Schedule new_schedule(schedule);
        new_schedule.Randomize(new_schedule.Jobs());
        Neighbor nb;

        clock_t tStart = clock();
        for (int tfind = 0; tfind < TIMES_TO_HYBRID; ++tfind)
        {
            nb = new_schedule.FindNeighbor(NEIGHBORHOOD_SIZE, tabulist);
            new_schedule.Visit(nb);
            tabulist.Push(nb);
        }
        tTotal += clock() - tStart;
        solution.Push(nb.getValue());
    }

    solution.Print(fout);

    fout << std::endl;
    fout << "Total time: " << (double)tTotal/CLOCKS_PER_SEC <<" sec"<< std::endl;

    return 0;
}