//**************************************************************
//         Test routine for move
//**************************************************************

#include <iostream>

using namespace std;

// std move implementation
template<typename T>
typename remove_reference<T>::type&&
Move(T&& t)
{
    using returnType = remove_reference<T>::type&&;
    return static_cast<returnType>(t);
}

void TestMove()
{
    int a = 1;
    //int& b = 1;// wrong
    int& b = a;
    int&& c = 1;
    //int&& c = b;// wrong
    
    static_assert(std::is_rvalue_reference<decltype(std::move(a))>::value, "the value is NOT rvalue reference after move.");
    static_assert(std::is_rvalue_reference<decltype(Move(a))>::value,"the value is NOT rvalue reference after move.");
}

class CtorCounter
{
public:
    CtorCounter() :m(new int(0))
    {
        cout << "Construct:" << ++nCtor << endl;
    }

    ~CtorCounter()
    {
        cout << "Deconstruct:" << ++nDtor << endl;
    }

    CtorCounter(CtorCounter& cc) :m(new int(*cc.m))
    {
        cout << "Copy Construct:" << ++nCptor << endl;
    }

    CtorCounter(CtorCounter&& cc) :m(cc.m)
    {
        cc.m = nullptr;
        cout << "Move Construct:" << ++nMctor << endl;
    }
public:
    static int nCtor;
    static int nDtor;
    static int nCptor;
    static int nMctor;

    int* m; // resource
};

int CtorCounter::nCtor = 0;
int CtorCounter::nDtor = 0;
int CtorCounter::nCptor = 0;
int CtorCounter::nMctor = 0;

CtorCounter GetObject()
{
    CtorCounter cc;
    cout << "Resource from: " << __FUNCTION__ << ": " << hex << cc.m << endl;
    //return std::move(cc);
    return cc;
}

void TestCtorCounter()
{
    CtorCounter cc = GetObject();
    cout << "Resource from: " << __FUNCTION__ << ": " << hex << cc.m << endl;
}
