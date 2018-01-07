//**************************************************************
//         std::auto_ptr alike smart pointer
//**************************************************************

#include "AutoPtr.h"
#include "SharedPtr.h"
#include <memory>
#include <iostream>

using namespace std;
// test object
class Base
{
public:
    Base(){};
    ~Base(){};

    void func()
    {
        cout << "Base func call." << endl;
    };
};

class Derive :public Base
{
public:
    Derive(){};
    ~Derive(){};
};

void AutoPtr_Test()
{
    int* p = new int(5);
    AutoPtr<int> sp(p);

    // copy operations will give up ownership
    AutoPtr<int> spc(sp);
    // note here will call copy constructor, not copy operator.
    // constructor will be invoked if object is not created.
    // otherwise AutoPtr<int> spcc; spcc = spc; will call constructor firstly
    // then copy operator.
    AutoPtr<int> spcc = spc;
    AutoPtr<int> spccc;
    spccc = spcc;

    // test for operator * and ->
    AutoPtr<Base> b(new Base);
    b->func();
    (*b).func();

    // multiple auto_ptr own same objects which delete raw pointer multiple times. Incorrect behavior.
    int* pp = new int(5);
    std::auto_ptr<int> ap(pp);
    std::auto_ptr<int> apc(ap);
    std::auto_ptr<int> apcc = apc;
}

void SharedPtr_Test()
{
    int* pp = new int(5);
    SharedPtr<int> sp(pp);
    SharedPtr<int> spp(sp);
    SharedPtr<int> sppp = spp;
    sppp.reset();

    //int* pp = new int(5);
    //std::shared_ptr<int> sp(pp);
    //std::shared_ptr<int> spother(pp); // anther shared ptr, ref count=1.
    //std::shared_ptr<int> spp(sp);
    //std::shared_ptr<int> sppp = spp;
    //sppp.reset();
}

int main()
{
    AutoPtr_Test();
    SharedPtr_Test();
}

