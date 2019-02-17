//**************************************************************
//         std::function
//**************************************************************

#ifndef FUNCTION_H
#define FUNCTION_H

#include <functional>
#include <iostream>

using namespace std;
using namespace std::placeholders;

// function
int func(int a, int b)
{
    return a + b;
}

// functor
class SomeFunctor
{
public:
    int operator()(int a, int b)
    {
        return a + b;
    }
};

// class member function
class SomeObject
{
public:
    int MemFunc(int a, int b)
    {
        return a + b;
    }

    static int StaticFunc(int a, int b)
    {
        return a + b;
    }
};

// template function
template<typename T>
T templateFunc(T a, T b)
{
    return a + b;
}

// test function is universal function accepts callable objects:
// normal function, functor, class member function, template function, lambda...
int test(int a, int b, const std::function<int(int, int)>& callableObject)
{
    return callableObject(a, b);
}

void TestFunction()
{
    // test normal function
    cout << "func(1,3): " << test(1, 3, func) << endl;

    // test functor
    SomeFunctor ftor;
    cout << "Functor(1,3): " << test(1, 3, ftor) << endl;

    // test class static function
    auto staticFunc = SomeObject::StaticFunc;
    cout << "SomeObject::StaticFunc(1,3): " << test(1, 3, staticFunc) << endl;

    // test class member function, member function needs to bind object this
    // to transform function object accepts two arguments.
	// Note: unlike static function which has unique function address, 
	// which is a callable function object directly.
	// to transfer a class member function to std::function, we need to 
	// know its address, which need to do bind firstly. Bind need to know
	// member function belongs to which class and which instance and also 
	// its signature(because overload functions may exist for this class?).
	// with these information, the class member function's address is determined
	// and can be called.
    auto memFunc = bind(&SomeObject::MemFunc, SomeObject(), _1, _2);
    cout << "SomeObject::MemFunc(1,3): " << test(1, 3, memFunc) << endl;

    // test template function
    cout << "templateFunc<int>(1,3): " << test(1, 3, templateFunc<int>) << endl;

    // test lambda
    cout << "[](int a, int b){ return a + b;}: " << test(1, 3, [](int a, int b){return a + b; }) << endl;
}

#endif
