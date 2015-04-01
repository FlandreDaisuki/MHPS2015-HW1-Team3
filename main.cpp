#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "datatype.h"

std::string finpath;
std::string foutpath;
Tabutype g_tabutype = Tabutype::ObjValue;
bool g_staticTabuSize = true;
int g_tabuSize = 50;
int g_tabuMaxSize = 0;
int g_iterationTimes = 1000;
int g_solutionNum = 50;

// ./a.out -i data/tai20_20_1.txt -o outputdata/tai20_20_1.txt -t value|pair -l 500|1000|2000|500~2000 -r 1000 -s 50
int main(int argc, char const *argv[])
{
    srand(time(NULL));
    
    int err = argset(argc, argv);
    if(err != 0)
    {
        return err;
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
    
    Schedule min_schedule(schedule);
    clock_t tTotal = 0;

    for (int sol = 0; sol < g_solutionNum; ++sol)
    {
        Tabulist tabulist(g_tabutype, g_tabuSize);
        Schedule new_schedule(schedule);
        new_schedule.Randomize(new_schedule.Jobs());
        Neighbor bestNeighbor;

        int onetenthIter = g_iterationTimes/10;
        int onetenthAddSize = (g_tabuMaxSize - g_tabuSize)/10;
        
        clock_t tStart = clock();
        for (int iter_t = 0; iter_t < g_iterationTimes; ++iter_t)
        {
            bestNeighbor = new_schedule.FindAllNeighbor(tabulist);
            new_schedule.Visit(bestNeighbor);
            tabulist.Push(bestNeighbor);
            if(!g_staticTabuSize && iter_t % onetenthIter == 0)
            {
                tabulist.addLimit(onetenthAddSize);
            }
        }
        tTotal += clock() - tStart;
        solution.Push(bestNeighbor);
        if(min_schedule.Calculate() > bestNeighbor.getValue()) {
            min_schedule = new_schedule;
        }
    }

    argPrint(fout);
    solution.Print(fout);
    min_schedule.Print(fout);

    fout << "Total time: " << (double)tTotal/CLOCKS_PER_SEC <<" sec"<< std::endl;

    return 0;
}