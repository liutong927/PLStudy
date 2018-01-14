//**************************************************************
//         std::auto_ptr alike smart pointer
//**************************************************************

#include "AutoPtr.h"
#include "SharedPtr.h"
#include "WeakPtr.h"
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
    cout << *(spp.get()) <<endl;

    // copy operator
    SharedPtr<int> a(new int(6));
    SharedPtr<int> b;
    b = a;


    //int* pp = new int(5);
    //std::shared_ptr<int> sp(pp);
    //std::shared_ptr<int> spother(pp); // anther shared ptr, ref count=1.
    //std::shared_ptr<int> spp(sp);
    //std::shared_ptr<int> sppp = spp;
    //cout << *(sppp.get());
    //sppp.reset();
}

void WeakPtr_Test()
{
    /*
    int* pp = new int(5);
    // weak_ptr cannot contruct from raw pointer.
    // std::weak_ptr<int> sp(pp);
    std::shared_ptr<int> sp = std::make_shared<int>(5);
    std::weak_ptr<int> wp(sp);
    //std::weak_ptr<int> wp2(wp);
    std::shared_ptr<int> spFromWeak = wp.lock();
    //wp.reset();
    */

    int* pp = new int(5);
    SharedPtr<int> sp(pp);
    WeakPtr<int> wp(sp);

}

struct B;
struct A
{
    ~A()
    {
        cout << "destroy A." << endl;
    }
    //std::shared_ptr<A> ptr;
    // SharedPtr<B> ptr;
    std::weak_ptr<B> ptr;
};

struct B
{
    ~B()
    {
        cout << "destroy B." << endl;
    }
    //std::shared_ptr<A> ptr;
    // SharedPtr<A> ptr;
    std::weak_ptr<A> ptr;
};

void CyclicReference_Test()
{
    //std::shared_ptr<A> p1 = std::make_shared<A>();
    //std::shared_ptr<A> p2 = std::make_shared<A>();

    //SharedPtr<A> p1(new A);// ref count=1
    //SharedPtr<B> p2(new B);// ref count=1

    //p1->ptr = p2;// ref count=2
    //p2->ptr = p1;// ref count=2
    // p1,p2 both decrease to 1 so never delete the resource.

    // using weak_ptr in A and B to solve cycle reference.
    std::shared_ptr<A> sp1 = std::make_shared<A>();
    std::shared_ptr<B> sp2 = std::make_shared<B>();

    sp1->ptr = sp2;// p2 has 1 strong reference, 1 weak reference
    sp2->ptr = sp1;// p1 has 1 stong ref, 1 weak ref.
}

int main()
{
    AutoPtr_Test();
    SharedPtr_Test();
    CyclicReference_Test();
    WeakPtr_Test();
}

