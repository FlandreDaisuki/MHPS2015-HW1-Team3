#include "datatype.h"

/*
 *  Class Neighbor
 */
Neighbor::Neighbor(): Neighbor(0, 0, 1e5)
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
void Neighbor::Print(std::ostream &out) const
{
    out << "(" << this->joba << " , " << this->jobb << " , " << this->objvalue << ")" << std::endl;
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
    if(nb.getValue() < best.getValue())
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

void Tabulist::Print(std::ostream &out) const
{
    out << "Tabulist:" << std::endl;
    for (const auto& nb: q)
    {
        out << nb.getValue() << " ";
    }
    out << std::endl;
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
void Schedule::Print(std::ostream &out) const
{
    out << "Jobs:" << this->job << " Machines:" << this->machine << std::endl;
    for (int i = 0; i < this->machine; ++i)
    {
        for (int j = 0; j < this->job; ++j)
        {
            out.width(2);
            out << this->matrix[i][j] <<" ";
        }
        out << std::endl;
    }
    out << "Span: " << Calculate() << std::endl << std::endl;
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
        
        if(cvalue < tabulist.Best().getValue() || !tabulist.inTabu(cvalue) && cvalue < best.getValue())
        {
            best.SetAll(joba, jobb, cvalue);
        }
        ss.SwapJobs(joba, jobb);
    }
    return best;
};
Neighbor Schedule::FindAllNeighbor(const Tabulist &tabulist) const
{
    Neighbor best;
    Schedule ss(*this);
    for (int joba = 0; joba < this->job-1; ++joba)
    {
        for (int jobb = joba + 1; jobb < this->job; ++jobb)
        {
            ss.SwapJobs(joba, jobb);

            int cvalue = ss.Calculate();
            
            if(cvalue < tabulist.Best().getValue() || !tabulist.inTabu(cvalue) && cvalue < best.getValue())
            {
                best.SetAll(joba, jobb, cvalue);
            }
            ss.SwapJobs(joba, jobb);
        }        
    }
    return best;
};

/*
 *  Class Solution
 */
Solution::Solution():max(-1), min(1e8), sum(0)
{
    //Nothing todo
};
Solution::~Solution()
{
    //Nothing todo
};
void Solution::Push(int a)
{
    v.push_back(a);
    max = std::max(max, a);
    min = std::min(min, a);
    sum += a;
};
void Solution::Print(std::ostream &out) const
{
    out << "Solution:" << std::endl;
    out << "Max: " << max << " min: " << min << " avg: " << (double)sum/v.size() << std::endl;
    for (int i = 0; i < v.size(); ++i)
    {
        out << v[i] <<" ";
    }
    out << std::endl;
};
int Solution::Max() const { return max; };
int Solution::Min() const { return min; };
int Solution::Size() const { return v.size(); };
int Solution::Sum() const { return sum; };