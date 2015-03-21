#include "datatype.h"

/*
 *  Class Neighbor
 */
Neighbor::Neighbor(): Neighbor(0, 0, 1e-5)
{
    //Nothing todo
};
Neighbor::Neighbor(int a,int b,int v) : joba(a), jobb(b), objvalue(v)
{
    // guarantee joba < jobb
    this->xchg();
};
Neighbor::Neighbor(const Neighbor &n) : Neighbor(n.joba, n.jobb, n.objvalue)
{
    //Nothing todo
};
Neighbor::~Neighbor()
{
    //Nothing todo
};
void Neighbor::SetAll(int a, int b, int v)
{
    this->joba = a;
    this->jobb = b;
    this->objvalue = v;
    this->xchg();
};
void Neighbor::Print() const
{
    std::cout << "(" << this->joba << " , " << this->jobb << " , " << this->objvalue << ")" << std::endl;
};
void Neighbor::xchg()
{
    if (this->joba > this->jobb)
    {
        std::swap(this->joba, this->jobb);
    }
};

/*
 *  Class Tabulist
 */
Tabulist::Tabulist(int n) : limit(n)
{
    //Nothing todo
};
Tabulist::~Tabulist()
{
    //Nothing todo
};
void Tabulist::Push(const Neighbor nb)
{
    q.push_back(nb);
    if(q.size() > limit)
    {
        q.pop_front();
    }
    if(nb.getValue() > best.getValue())
    {
        best = nb;
    }
}
void Tabulist::Pop()
{
    if(!q.empty())
    {
        q.pop_front();
    }
}

const Neighbor& Tabulist::Best() const
{
    return this->best;
}

bool Tabulist::inTabu(int value) const
{
    for (const auto& nb: q)
    {
        if(nb.getValue() == value)
        {
            return true;
        }
    }
    return false;
}

/*
 *  Class Schedule
 */
Schedule::Schedule(int j, int m) : job(j), machine(m)
{
    this->matrix.resize(machine, std::vector<int> (job,0));
};
Schedule::Schedule(const Schedule &s) : job(s.job), machine(s.machine)
{
    this->matrix.resize(machine, std::vector<int> (job,0));

    for (int i = 0; i < s.machine; ++i)
    {
        for (int j = 0; j < s.job; ++j)
        {
            this->matrix[i][j] = s.matrix[i][j];
        }
    }
};
Schedule::~Schedule()
{
    //Nothing todo
};
void Schedule::Print() const
{
    std::cout << "Jobs:" << this->job << " Machines:" << this->machine << std::endl;
    for (int i = 0; i < this->machine; ++i)
    {
        for (int j = 0; j < this->job; ++j)
        {
            std::cout.width(2);
            std::cout << this->matrix[i][j] <<" ";
        }
        std::cout << std::endl;
    }
};
void Schedule::SwapJobs(int a, int b)
{
    for (int i = 0; i < this->machine; ++i)
    {
        std::swap(matrix[i][a],matrix[i][b]);
    }
};
void Schedule::Visit(const Neighbor &nb)
{
    this->SwapJobs(nb.getJobA(), nb.getJobB());
};
int Schedule::Calculate() const
{
    std::vector<int> timespan(job+1, 0);

    for (int i = 0; i < machine; ++i)
    {
        for (int j = 1; j <= job; ++j)
        {
            timespan[j] = std::max(timespan[j], timespan[j-1]) + matrix[i][j-1];
        }
    }
    return timespan[job];
};
Neighbor Schedule::FindNeighbor(int n, const Tabulist &tabulist) const
{
    const int N_NEIGHBOR = n;
    Neighbor best;
    Schedule ss(*this);
    for (int i = 0; i < N_NEIGHBOR; ++i)
    {
        int joba = rand()%this->job;
        int jobb;
        do
        {
            jobb = rand()%this->job;
        } while (joba == jobb);
        ss.SwapJobs(joba, jobb);

        int cvalue = ss.Calculate();
        
        if(cvalue > tabulist.Best().getValue() || !tabulist.inTabu(cvalue) && cvalue > best.getValue())
        {
            best.SetAll(joba, jobb, cvalue);
        }
        ss.SwapJobs(joba, jobb);
    }
    return best;
};