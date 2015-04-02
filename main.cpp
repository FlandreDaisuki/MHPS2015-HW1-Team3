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
    

    for (int i = 0; i < machine; ++i)
    {
        for (int j = 0; j < job; ++j)
        {
            fin >> schedule.getMatrix()[i][j];
        }
    }
    
    Schedule g_min_schedule(schedule);
    std::vector<int> min_iterationList;
    Solution solution;

    int onetenthIter = g_iterationTimes/10;
    int onetenthAddSize = (g_tabuMaxSize - g_tabuSize)/10;

    clock_t tTotal = 0;
    for (int sol_t = 0; sol_t < g_solutionNum; ++sol_t)
    {
        Tabulist tabulist(g_tabutype, g_tabuSize);
        Schedule new_schedule(schedule);
        Schedule loc_min_schedule(schedule);
        new_schedule.Randomize(new_schedule.Jobs());
        Neighbor min_neighbor;
        std::vector<int> new_iterationList;

        clock_t tStart = clock();
        for (int iter_t = 0; iter_t < g_iterationTimes; ++iter_t)
        {
            Neighbor nb = new_schedule.FindAllNeighbor(tabulist); // return a best neighbor
            new_schedule.Visit(nb);                               // visit to the above neighbor
            tabulist.Push(nb);                                    // tabu the neighbor
            new_iterationList.push_back(nb.getValue());           // log makespan of iteration

            if(!g_staticTabuSize && iter_t % onetenthIter == 0)   // if variable size add some limit 
            {
                tabulist.addLimit(onetenthAddSize);
            }

            if (nb.getValue() < min_neighbor.getValue())          // log the minimum one
            {
                min_neighbor = nb;
                loc_min_schedule = new_schedule;
            }
        }
        tTotal += clock() - tStart;
        solution.Push(min_neighbor);
        if(min_neighbor.getValue() == solution.Min())
        {
            g_min_schedule = loc_min_schedule;
            min_iterationList = new_iterationList;
        }
    }

    argPrint(fout);
    solution.Print(fout);
    g_min_schedule.Print(fout);
    
    std::ofstream csvout;
    csvout.open(("csv/"+fname+".csv").c_str());
    for(size_t i = 0 ; i < min_iterationList.size() ; ++i)
    {
        csvout << min_iterationList[i] << std::endl;
    }

    fout << "Total time: " << (double)tTotal/CLOCKS_PER_SEC <<" sec"<< std::endl;

    return 0;
}