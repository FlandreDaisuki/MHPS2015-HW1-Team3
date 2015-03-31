#pragma once

#include <vector>
#include <iostream>
#include <ostream>
#include <utility>
#include <deque>
#include <algorithm>

typedef std::vector< std::vector<int> > Matrix;

class Neighbor
{
public:
    Neighbor();
    Neighbor(int a,int b,int v);
    Neighbor(const Neighbor &n);
    ~Neighbor();
    int getValue() const { return objvalue; }
    int getJobA() const { return joba; }
    int getJobB() const { return jobb; }
    void SetAll(int a,int b,int v);
    void Print(std::ostream &out = std::cout) const;
private:
    void xchg();
    int joba, jobb, objvalue;
};

class Tabulist
{
public:
    Tabulist(int n);
    ~Tabulist();

    void Push(const Neighbor nb);
    void Pop();

    const Neighbor& Best() const;
    bool inTabu(int value) const;
    void Print(std::ostream &out = std::cout) const;
private:
    std::deque<Neighbor> q;
    Neighbor best;
    int limit;
};

class Schedule
{
public:
    Schedule(int j, int m);
    Schedule(const Schedule &s);
    ~Schedule();
    
    Matrix &getMatrix() { return matrix; }
    const Matrix &getMatrix() const { return matrix; }
    void Print(std::ostream &out = std::cout) const;
    int Jobs() const { return job; }
    int Machines() const { return machine; }
    void Randomize(int n);
    void SwapJobs(int a, int b);
    void Visit(const Neighbor &nb);
    int Calculate() const;
    Neighbor FindNeighbor(int n, const Tabulist &tabulist) const;
    Neighbor FindAllNeighbor(const Tabulist &tabulist) const;
private:
    int job, machine;
    Matrix matrix;
};

class Solution
{
public:
    Solution();
    ~Solution();
    void Push(int a);
    void Print(std::ostream &out = std::cout) const;
    int Max() const;
    int Min() const;
    int Size() const;
    int Sum() const;
private:
    std::vector<int> v;
    int max;
    int min;
    int sum;
};