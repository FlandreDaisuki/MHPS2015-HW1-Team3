#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

const int BUFFER_SIZE = 1024;
char buffer[BUFFER_SIZE];

void NewArray2D(int **&a, int r, int c);
void DelArray2D(int **&a, int c);
int Objective(int **dp, int mac, int job);
void SwapJob(int **a, int mac, int ja, int jb);

int main(int argc, char const *argv[])
{
    int machine_num, job_num;
    char filename[15];
    cin.getline(buffer, BUFFER_SIZE);
    sscanf(buffer, " %d %d %s", &job_num, &machine_num, filename);
    int **table;
    NewArray2D(table, machine_num, job_num);
    int **dp;
    NewArray2D(dp, machine_num, job_num);
    for (int i = 0; i < machine_num; ++i)
    {
        for (int j = 0; j < job_num; ++j)
        {
            cin >> table[i][j];
            dp[i][j] = table[i][j];
        }
    }

    return 0;
}

void NewArray2D(int **&a, int r, int c)
{
    a = new int*[r];
    for (int i = 0; i < r; ++i)
    {
        a[i] = new int[c];
    }
}

void DelArray2D(int **&a, int c)
{
    for (int i = 0; i < c; ++i)
    {
        delete [] a[i];
    }
    delete [] a;
}

int Objective(int **dp, int mac, int job)
{
    for (int i = 0; i < mac; ++i)
    {
        for (int j = 0; j < job; ++j)
        {
            int m = (i > 0) ? dp[i-1][j] : 0;
            int n = (j > 0) ? dp[i][j-1] : 0;
            
            dp[i][j] += max(m,n); 
        }
    }
    return dp[mac-1][job-1];
}

void SwapJob(int **a, int mac, int ja, int jb)
{
    for(int i = 0; i < mac; ++i)
    {
        swap(a[i][ja], a[i][jb]);
    }
}

void PrintTable(int **a, int mac, int job)
{
    for (int i = 0; i < mac; ++i)
    {
        for (int j = 0; j < job; ++j)
        {
            cout << a[i][j];
        }
    }
}