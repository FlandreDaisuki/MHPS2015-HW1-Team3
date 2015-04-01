#include "datatype.h"

int argset(int argc, char const *argv[])
{
    for (int i = 1; i < argc && i+1 < argc; i+=2)
    {
        std::string attr(argv[i]);
        std::string val(argv[i+1]);
        if (attr == "-i")
        {
            finpath = val;
        }
        else if(attr == "-o")
        {
            foutpath = val;
        }
        else if(attr == "-t") //tabu type
        {
            if (val == "pair")
            {
                g_tabutype = Tabutype::Pair;
            }
        }
        else if(attr == "-l") //tabu list size
        {
            int sizedet = sscanf(val.c_str(),"%d%*c%d",&g_tabuSize,&g_tabuMaxSize);
            g_staticTabuSize = (sizedet == 1);
        }
        else if(attr == "-r")
        {
            sscanf(val.c_str(),"%d",&g_iterationTimes);
        }
        else if(attr == "-s")
        {
            sscanf(val.c_str(),"%d",&g_solutionNum);
        }
        else
        {
            return -1;
        }
    }

    if(finpath == "")
    {
        return -1;
    }
    if(finpath != "" && foutpath == "")
    {
        foutpath = "output" + finpath;
    }
    return 0;
}
void argPrint(std::ostream &out)
{   
    using std::endl;
    out << "Tabutype: " << ((g_tabutype == Tabutype::ObjValue) ? "Objective Value":"Swap Pair") << endl;
    out << "Tabulist Size: " << g_tabuSize;
    if (!g_staticTabuSize) { out << " ~ " << g_tabuMaxSize; }
    out << endl << "Iteration Times:" << g_iterationTimes << endl;
    out << "Solution Num:" << g_solutionNum << endl << endl;
}

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
Tabulist::Tabulist(Tabutype t,int n) : type(t),limit(n)
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
        best.SetAll(nb.getJobA(),nb.getJobB(),nb.getValue());
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
bool Tabulist::inTabu(const Neighbor &testn) const
{
    if(this->type == Tabutype::ObjValue)
    {
        for (const auto& nb: q)
        {
            if(nb.getValue() == testn.getValue())
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        for (const auto& nb: q)
        {
            if(nb.getJobA() == testn.getJobA() && nb.getJobB() == testn.getJobB())
            {
                return true;
            }
        }
        return false;
    }
}
void Tabulist::addLimit(int n)
{
    this->limit += n;
}
void Tabulist::Print(std::ostream &out) const
{
    out << "Tabulist:" << std::endl;
    for (const auto& nb: q)
    {
        nb.Print(out);
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
Neighbor Schedule::FindAllNeighbor(const Tabulist &tabulist) const
{
    Neighbor best, temp;
    Schedule ss(*this);
    for (int joba = 0; joba < this->job-1; ++joba)
    {
        for (int jobb = joba + 1; jobb < this->job; ++jobb)
        {
            ss.SwapJobs(joba, jobb);

            int cvalue = ss.Calculate();
            temp.SetAll(joba, jobb, cvalue);

            if(cvalue < tabulist.Best().getValue() || !tabulist.inTabu(temp) && cvalue < best.getValue())
            {
                best.SetAll(joba, jobb, cvalue);
            }
            ss.SwapJobs(joba, jobb);
        }        
    }
    return best;
};
void Schedule::Randomize(int n) {
    for (int i = 0; i < n; ++i)
    {
        int joba = rand()%this->job;
        int jobb;
        do
        {
            jobb = rand()%this->job;
        } while (joba == jobb);
        this->SwapJobs(joba, jobb);
    }
}
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
void Solution::Push(const Neighbor &n)
{
    int a = n.getValue();
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
    out << std::endl << std::endl;
};
int Solution::Max() const { return max; };
int Solution::Min() const { return min; };
int Solution::Size() const { return v.size(); };
int Solution::Sum() const { return sum; };