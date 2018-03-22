//**************************************************************
//         Test routine for move
//**************************************************************

#include <iostream>

using namespace std;

// std move implementation
// std::move(x) is declared as an rvalue reference and does not have a name.Hence, it is an rvalue.

// <clarification from "Effective Modern C++">
// The ¡°&&¡± part of the function¡¯s return type implies that std::move returns an rvalue
// reference, but if the type T happens to be an lvalue reference, T&& would become an lvalue reference.
// To prevent this from happening, the type trait std::remove_reference is applied to T,
// thus ensuring that ¡°&&¡± is applied to a type that isn¡¯t a reference.
// That guarantees that std::move truly returns an rvalue reference, and that¡¯s important,
// because rvalue references returned from functions are rvalues.
// Thus, std::move casts its argument to an rvalue, and that¡¯s all it does.
template<typename T>
typename remove_reference<T>::type&&
Move(T&& t)
{
    using returnType = remove_reference<T>::type&&;
    return static_cast<returnType>(t);
}

void TestFunc(int&&)
{

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

void FuncRvalRef(CtorCounter&& cc)
{
    // cpctor or mctor will be called?
    // it depends on whether cc is lvalue or rvalue.
    // cc has name, so it is lvalue, call cpctor.
    CtorCounter temp = cc;
}

void TestMove()
{
    int a = 1;//lvalue
    //int& b = 1;// wrong, lvalue reference cannot bind to rvalue.
    int& b = a;//lvalue reference
    int* address = &b;//Note that b itself is lvalue but with lvalue reference type
    int&& c = 1;//rvalue reference
    int* address2 = &c;//Note that c itself is lvalue but with rvalue reference type
    //int&& c = b;// wrong, rvalue reference cannot bind to lvalue.
    int&& d = a++;

    static_assert(!std::is_reference<decltype(a)>::value, "the value is reference.");
    static_assert(!std::is_reference<decltype(a++)>::value, "the value is reference.");
    static_assert(std::is_lvalue_reference<decltype(b)>::value, "the value is NOT lvalue reference.");
    static_assert(std::is_rvalue_reference<decltype(c)>::value, "the value is NOT rvalue reference.");
    static_assert(std::is_rvalue_reference<decltype(std::move(a))>::value, "the value is NOT rvalue reference after move.");
    static_assert(std::is_rvalue_reference<decltype(Move(a))>::value, "the value is NOT rvalue reference after move.");

    TestFunc(1);
    //TestFunc(a);// wrong, a is lvalue
    //TestFunc(c);// wrong, why? type of c is int&&, why cannot call TestFunc?
                  // this is because c has a name, is lvalue.
    TestFunc(std::move(c));
    //TestFunc(d);// wrong, same with TestFunc(c).

    // Importance: rvalue reference is not necessarily rvalue.
    // Things that are declared as rvalue reference can be lvalues or rvalues. 
    // The distinguishing criterion is: if it has a name, then it is an lvalue. Otherwise, it is an rvalue.
    FuncRvalRef(CtorCounter());
}

