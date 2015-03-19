#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "datatype.h"

Neighbor FindNeighbor(const Schedule &schedule, const Tabulist &tabulist);
void VisitNeighbor(Schedule &schedule, const Neighbor &nb);
void Tabu(Tabulist &tabulist, const Neighbor &nb) {}
int Calculate(const Schedule &schedule);

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    const int HOW_TIMES_TO_RUN = 20;
    int job, machine;
    std::string fname;
    //std::cin >> job >> machine >> fname;
    std::ifstream fin;
    fin.open("data/debug");

    fin >> job >> machine >> fname;

    Schedule schedule(job, machine);
    Solution solution;

    for (int i = 0; i < machine; ++i)
    {
        for (int j = 0; j < job; ++j)
        {
            //std::cin >> schedule.getMatrix()[i][j];
            fin >> schedule.getMatrix()[i][j];
        }
    }
    schedule.Print();
    std::cout << "Total: " << Calculate(schedule) << std::endl << std::endl;
    Tabulist tabulist;

    Neighbor nb = FindNeighbor(schedule, tabulist);
    nb.Print();
    solution.push_back(nb.getValue());
    VisitNeighbor(schedule, nb);
    Tabu(tabulist, nb);

    return 0;
}

Neighbor FindNeighbor(const Schedule &schedule, const Tabulist &tabulist)
{
    const int N_NEIGHBOR = 3;
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
        if(cvalue > best.getValue())
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
    //std::cout << std::endl;
    for (int i = 0; i < schedule.Machines(); ++i)
    {
        for (int j = 1; j <= schedule.Jobs(); ++j)
        {
            timespan[j] = std::max(timespan[j], timespan[j-1]) + schedule.getMatrix()[i][j-1];
            //std::cout << timespan[j] << " ";
        }
        //std::cout << std::endl;
    }
    return timespan[schedule.Jobs()];
}

void VisitNeighbor(Schedule &schedule, const Neighbor &nb)
{
    schedule.Swap(nb.getJobA(),nb.getJobB());
}
