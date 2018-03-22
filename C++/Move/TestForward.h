//**************************************************************
//         Test routine for perfect forwarding
//**************************************************************

#include <algorithm>
#include <iostream>

using namespace std;

// Do we need && version?

// Forward is a conditional cast, cast to rvalue only if its arguments was
// initialized with an rvalue.
template<typename T>
T&& Forward(typename std::remove_reference<T>::type& t)
{
    return static_cast<T&&>(t);
}

template<typename T1, typename T2>
void Func(T1& t1, T2& t2)
{
    cout << "lvalue of Func called" << endl;
}

template<typename T1, typename T2>
void Func(T1&& t1, T2&& t2)
{
    cout << "rvalue of Func called" << endl;
}

// simple wrapper only accepts lvalue.
template<typename T1, typename T2>
void Wrapper(T1& t1, T2& t2)
{
    Func(t1, t2);
}

// const reference wrapper accepts both lvalue and rvalue.
// but with 2 main issues"
// 1. scalability issue which needs provide all combinations of const and non-const
//    reference for various arguments.
// 2. not perfect forwarding. t1/t2 in the body is lvalue, which block move semantics.
//    lvalue version of Func will always be called even argument is rvalue.
template<typename T1, typename T2>
void WrapperConst(const T1& t1, const T2& t2)
{
    Func(t1, t2);
}

// perfect wrapper accepts both lvalue and rvalue,
// and use perfect forwarding to forward functions depends on arguments type.
template<typename T1, typename T2>
void WrapperPerfect(T1&& t1, T2&& t2)
{
    //Func(std::forward<T1>(t1), std::forward<T2>(t2));
    Func(Forward<T1>(t1), Forward<T2>(t2));
}

void TestForward()
{
    // 1. rvalue cannot be bound to Wrapper since it accepts only reference.
    //Wrapper(4, 5);
    // 2. WrapperConst can make it pass, but Func may still need to change t1 and t2,
    WrapperConst(4, 5);// cout lvalue version of Func
    // 3. So overload const and non-const version of Wrapper which is not a scalable solution.
    // also not work for rvalue reference &&.
    // until now, we have problem with perfect forwarding.

    // 4. use std::forward to solve perfect forwarding issue.
    int a, b;//lvalue
    // T1 deduced to int&, Forward is instantiated with int&,
    // int& && Forward(int&)
    //{
    //    return static_cast<int& &&>(t);
    //}
    // further
    // int& Forward(int&)
    //{
    //    return static_cast<int&>(t);
    //}
    // so by-reference argument is perfectly forwarding for lvalue.
    WrapperPerfect(a, b);// cout lvalue version of Func

    // now for the rvalue.
    // T1 is deduce to int, Forward is instantiated with int,
    // int && Forward(int&)
    //{
    //    return static_cast<int&&>(t);
    //}
    // by-reference argument is perfectly forward to rvalue reference
    WrapperPerfect(4, 5);// cout rvalue version of Func

    WrapperPerfect(std::move(a), std::move(b));// cout rvalue version of Func
}

