//**************************************************************
//         lambda
//**************************************************************

#ifndef LAMBDA_H
#define LAMBDA_H

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// test data set and utilities.
const int gBound = 5;
vector<int> rawDataSet = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
vector<int> qualifiedDataSet;

void Print(vector<int>& vec)
{
    for (auto i : vec)
    {
        cout << i << " ";
    }
    cout << endl;
}

// several ways to filter raw data set.
// 1. normal filter method
void FindWithLegacyWay()
{
    for (auto i : rawDataSet)
    {
        if (i > gBound)
        {
            qualifiedDataSet.push_back(i);
        }
    }

    Print(qualifiedDataSet);
    qualifiedDataSet.clear();
}

// 2. for_each + function
void Find(int i)
{
    if (i > gBound)
        qualifiedDataSet.push_back(i);
}

void FindWithFunction()
{
    for_each(rawDataSet.begin(), rawDataSet.end(), Find);

    Print(qualifiedDataSet);
    qualifiedDataSet.clear();
}

// 3. functor
class Functor
{
public:
    Functor(int bd) :bound(bd)
    {
    }

    void operator()(int i)
    {
        if (i > bound)
        {
            qualifiedDataSet.push_back(i);
        }
    }
private:
    int bound;
};

void FindWithFunctor()
{
    for_each(rawDataSet.begin(), rawDataSet.end(), Functor(gBound));

    Print(qualifiedDataSet);
    qualifiedDataSet.clear();
}

// 4. lambda
void FindWithLambda()
{
    for_each(rawDataSet.begin(), rawDataSet.end(), [=](int i){if (i > gBound){ qualifiedDataSet.push_back(i); }});

    Print(qualifiedDataSet);
    qualifiedDataSet.clear();
}

// compare several ways.
void TestLambda()
{
    FindWithLegacyWay();
    FindWithFunction();
    FindWithFunctor();
    FindWithLambda();
}

#endif
