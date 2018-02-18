//**************************************************************
//         std::bind
//**************************************************************

#ifndef BIND_H
#define BIND_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::placeholders;

int Add(int a, int b)
{
    return a + b;
}

int Minus(int a, int b)
{
    return a - b;
}

void TestBindFunction()
{
    // bind simple function with n arguments to function objects with 0~n argument.
    auto f_add = bind(Add, 1, 2);
    //std::function<int(void)> f_add = bind(Add, 1, 2);
    //cout << typeid(f_add).name() << endl;
    cout << f_add() << endl;

    auto f_add2 = bind(Add, _1, 2);
    //std::function<int(int)> f_add2 = bind(Add, 1, 2);
    //cout << typeid(f_add2).name() << endl;
    cout << f_add2(1) << endl;

    auto f_add3 = bind(Add, _1, _2);
    //std::function<int(int)> f_add3 = bind(Add, 1, 2);
    //cout << typeid(f_add3).name() << endl;
    cout << f_add3(1, 2) << endl;

    auto f_minus = bind(Minus, _1, _2);
    auto f_minus2 = bind(Minus, _2, _1);
    cout << f_minus(2, 1) << endl;
    cout << f_minus2(2, 1) << endl;
}

class Multiply
{
public:
    Multiply(int a, int b) :a(a), b(b)
    {
    }

    int mul()
    {
        return a*b;
    }
public:
    int a;
    int b;
};

void TestBindClassMem()
{
    Multiply m(2, 3);

    auto ftor_memfun = bind(&Multiply::mul, _1);
    cout << ftor_memfun(m) << endl;

    // pass by value
    auto ftor_memdata = bind(&Multiply::a, m);
    cout << ftor_memdata() << endl;
    ftor_memdata() = 5;
    cout << m.a << endl;
    cout << ftor_memdata() << endl;

    // placeholder pass by ref
    auto ftor_memdata2 = bind(&Multiply::b, _1);
    cout << ftor_memdata2(m) << endl;
    ftor_memdata2(m) = 6;
    cout << m.b << endl;
    cout << ftor_memdata2(m) << endl;
}

void TestBindSTD()
{
    vector<int> v{ 2, 4, 6, 8, 10, 11, 15, 20, 25 };
    // bind first argument with 9, so count for element meets 9<i.
    cout << count_if(v.begin(), v.end(), bind1st(less<int>(), 9)) << endl;
    // bind second argument with 9, so count for element meets i<9.
    cout << count_if(v.begin(), v.end(), bind2nd(less<int>(), 9)) << endl;
    // using bind
    cout << count_if(v.begin(), v.end(), bind(less<int>(), 9, _1)) << endl;
    cout << count_if(v.begin(), v.end(), bind(less<int>(), _1, 9)) << endl;
}

void TestBind()
{
    TestBindFunction();
    TestBindClassMem();
    TestBindSTD();
}

#endif
