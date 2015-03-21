#pragma once

#include <vector>
#include <iostream>
#include <utility>
#include <deque>

typedef std::vector<int> Solution;
typedef std::vector< std::vector<int> > Matrix;

class Neighbor
{
public:
    Neighbor() : Neighbor(0, 0, 1e-5) {};
    Neighbor(int a,int b,int v) : joba(a), jobb(b), objvalue(v)
    {
        // guarantee joba < jobb
        if (a>b)
        {
            std::swap(this->joba, this->jobb);
        }
    };
    Neighbor(const Neighbor &n) : Neighbor(n.joba, n.jobb, n.objvalue) {};
    ~Neighbor() {};
    int getValue() const { return objvalue; }
    int getJobA() const { return joba; };
    int getJobB() const { return jobb; };
    void SetAll(int a,int b,int v)
    {
        this->joba = a;
        this->jobb = b;
        this->objvalue = v;
        if (a>b)
        {
            std::swap(this->joba, this->jobb);
        }
    }
    void Print() const
    {
        std::cout << "(" << this->joba << " , " << this->jobb << " , " << this->objvalue << ")" << std::endl;
    }
private:
    int joba, jobb, objvalue;
};

class Schedule
{
public:
    Schedule(int j, int m) : job(j), machine(m)
    {
        this->matrix.resize(machine, std::vector<int> (job,0));
    };
    Schedule(const Schedule &s) : job(s.job), machine(s.machine)
    {
        this->matrix.resize(machine, std::vector<int> (job,0));

        for (int i = 0; i < s.machine; ++i)
        {
            for (int j = 0; j < s.job; ++j)
            {
                this->matrix[i][j] = s.matrix[i][j];
            }
        }
    }
    ~Schedule() {};
    
    Matrix &getMatrix() { return matrix; }
    const Matrix &getMatrix() const { return matrix; }
    void Print()
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
    }
    int Jobs() const { return job; }
    int Machines() const { return machine; }
    void Swap(int a, int b)
    {
        for (int i = 0; i < this->machine; ++i)
        {
            std::swap(matrix[i][a],matrix[i][b]);
        }
    }
    void Visit(const Neighbor &nb)
    {
        this->Swap(nb.getJobA(),nb.getJobB());
    }
private:
    int job, machine;
    Matrix matrix;
};

class Tabulist
{
public:
    Tabulist(int n) : limit(n) {}
    ~Tabulist() {};
    void Push(const Neighbor nb)
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
    void Pop()
    {
        if(!q.empty())
        {
            q.pop_front();
        }
    }

    const Neighbor& Best() const
    {
        return this->best;
    }

    std::deque<Neighbor>& List() { return q; }
    bool inTabu(int value) const
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
private:
    std::deque<Neighbor> q;
    Neighbor best;
    int limit;
};