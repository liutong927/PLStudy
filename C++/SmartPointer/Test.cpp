//**************************************************************
//          smart pointer test cases
//**************************************************************

#include "AutoPtr.h"
#include "SharedPtr.h"
#include "WeakPtr.h"
#include "UniquePtr.h"
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
    sppp.Reset();
    cout << *(spp.Get()) <<endl;

    // copy operator
    SharedPtr<int> a(new int(6));
    SharedPtr<int> b;
    b = a;

    // std::shared_ptr test
    int* x = new int(5);
    std::shared_ptr<int> spx(x);
    std::shared_ptr<int> spxx(spx); // anther shared ptr, ref count=1.
    std::shared_ptr<int> spxxx = spxx;
    cout << *(spxxx.get()) << endl;
    spxxx.reset();
}

void WeakPtr_Test()
{
    // int* a = new int(5);
    // weak_ptr cannot contruct from raw pointer.
    // std::weak_ptr<int> sp(a);
    //std::shared_ptr<int> sp_std = std::make_shared<int>(5);
    //std::weak_ptr<int> wp_std(sp_std);
    //std::weak_ptr<int> wp2(wp);
    //std::shared_ptr<int> spFromWeak = wp_std.lock();
    //wp_std.reset();


    int* pp = new int(5);
    SharedPtr<int> sp(pp);
    WeakPtr<int> wp(sp);
    SharedPtr<int> splock = wp.Lock();
    wp.Reset();
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
    //std::weak_ptr<B> ptr;
    WeakPtr<B> ptr;
};

struct B
{
    ~B()
    {
        cout << "destroy B." << endl;
    }
    //std::shared_ptr<A> ptr;
    // SharedPtr<A> ptr;
    //std::weak_ptr<A> ptr;
    WeakPtr<A> ptr;
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
    //std::shared_ptr<A> sp1 = std::make_shared<A>();
    //std::shared_ptr<B> sp2 = std::make_shared<B>();

    //sp1->ptr = sp2;// p2 has 1 strong reference, 1 weak reference
    //sp2->ptr = sp1;// p1 has 1 stong ref, 1 weak ref.

    SharedPtr<A> p1(new A);// ref count=1
    SharedPtr<B> p2(new B);// ref count=1

    p1->ptr = p2;// ref count=2
    p2->ptr = p1;// ref count=2
}

void UniquePtr_Test()
{
    int* p = new int(5);
    UniquePtr<int> up(p);
    UniquePtr<int> up2(std::move(up));
    //UniquePtr<int> up2(up);//wrong
    //UniquePtr<int> up3 = up;//wrong

    std::unique_ptr<int> sup = std::make_unique<int>(6);
    std::unique_ptr<int> sup2(std::move(sup));
    std::unique_ptr<int> sup3 = std::move(sup);
}

int main()
{
    //AutoPtr_Test();
    //SharedPtr_Test();
    //CyclicReference_Test();
    //WeakPtr_Test();
    UniquePtr_Test();
}

