#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "datatype.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    const int HOW_TIMES_TO_RUN = 20;
    int job, machine;
    std::string fname;
    std::ifstream fin;
    fin.open("data/tai20_5_1.txt");

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
    std::cout << "Total: " << schedule.Calculate() << std::endl << std::endl;
    

    const int TIMES_TO_FIND = 300;
    const int SOLUTION_TO_GEN = 10;
    
    for (int sol = 0; sol < SOLUTION_TO_GEN; ++sol)
    {
        Tabulist tabulist(5);
        Schedule new_schedule(schedule);
        Neighbor nb;
        for (int tfind = 0; tfind < TIMES_TO_FIND; ++tfind)
        {
            nb = new_schedule.FindNeighbor(20, tabulist);
            new_schedule.Visit(nb);
            tabulist.Push(nb);
        }
        solution.push_back(nb.getValue());
    }

    std::cout << "Solution: ";
    for (int i = 0; i < solution.size(); ++i)
    {
        std::cout << solution[i] <<" ";
    }
    std::cout << std::endl;

    return 0;
}