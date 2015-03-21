#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "datatype.h"

Neighbor FindNeighbor(const Schedule &schedule, const Tabulist &tabulist);
int Calculate(const Schedule &schedule);

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
    std::cout << "Total: " << Calculate(schedule) << std::endl << std::endl;
    

    const int TIMES_TO_FIND = 300;
    const int SOLUTION_TO_GEN = 10;
    
    for (int sol = 0; sol < SOLUTION_TO_GEN; ++sol)
    {
        Tabulist tabulist(5);
        Schedule new_schedule(schedule);
        Neighbor nb;
        for (int tfind = 0; tfind < TIMES_TO_FIND; ++tfind)
        {
            nb = FindNeighbor(new_schedule, tabulist);
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

    return 0;
}

Neighbor FindNeighbor(const Schedule &schedule, const Tabulist &tabulist)
{
    const int N_NEIGHBOR = 4;
    Neighbor best;
    Schedule ss(schedule);
    for (int i = 0; i < N_NEIGHBOR; ++i)
    {
        int joba = rand()%schedule.Jobs();
        int jobb;
        do
        {
            jobb = rand()%schedule.Jobs();
        } while (joba == jobb);
        ss.Swap(joba, jobb);

        int cvalue = Calculate(ss);

        if(!tabulist.inTabu(cvalue) && cvalue > best.getValue() || cvalue > tabulist.Best().getValue())
        {
            best.SetAll(joba,jobb,cvalue);
        }
        ss.Swap(joba, jobb);
    }
    return best;
}

int Calculate(const Schedule &schedule)
{
    std::vector<int> timespan(schedule.Jobs()+1, 0);

    for (int i = 0; i < schedule.Machines(); ++i)
    {
        for (int j = 1; j <= schedule.Jobs(); ++j)
        {
            timespan[j] = std::max(timespan[j], timespan[j-1]) + schedule.getMatrix()[i][j-1];
        }
    }
    return timespan[schedule.Jobs()];
}