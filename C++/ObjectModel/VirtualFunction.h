//**************************************************************
//         virtual function study
//**************************************************************

#include <iostream>

using namespace std;

// runtime polymorphism represents addressing of a derived class object through 
// a pointer or reference of a public base class.

class Base
{
public:
    virtual void func1(){};
    virtual void func2()
    {
        cout << "Base func2 invoked." << endl;
    };
    virtual void func3(){};

    ~Base()
    {
        cout << "Base deconstruct." << endl;
    };
};

class Derived :public Base
{
public:
    virtual void func2()
    {
        cout << "Derived func2 invoked." << endl;
    };

    ~Derived()
    {
        cout << "Derived deconstruct." << endl;
    };
};

class Derived2 :public Derived
{
public:
    ~Derived2()
    {
        cout << "Derived2 deconstruct." << endl;
    };
};

void TestVirtualFunction()
{
    // each object has virtual function pointer(vfptr) 
    // which point to virtual table contains virtual function address.
    // pB1 and pB2's vfptr points to vtbl of Base:
    // address of Base::func1, Base::func2, Base::func3.
    // pB1 and pB2's vfptr points to vtbl of Derived:
    // address of Base::func1, Dervied::func2, Base::func3.
    Base* pB1 = new Base;
    Base* pB2 = new Base;
    Base* pD1 = new Derived;
    Base* pD2 = new Derived;

    // pointer to Base may point to Base or Derived object.
    // if it is Base object, then calling Base::func2().
    // if it is Derived object, then calling Derived::func2().
    pB2->func2();
    pD2->func2();

    // virtual function only takes effect on pointer/reference.
    // object still calls functions belong to itself.
    Base b;
    cout << sizeof(Base) << endl;
    b.func2();

    Derived d;
    cout << sizeof(Derived) << endl;
    d.func2();

    // although derived object is assigned to base object,
    // b is still base object. Calling b.func2() will still call Base::func2().
    b = d;
    b.func2();
}

void TestDeconstructor()
{
    // pB points to Base object, if dctor is non-virtual,
    // then this Base object does not have its virtual function address.
    // delete this object only calls ~Base().
    Base* pB = new Derived;
    delete pB;

    // if dctor is non-virtual, delete Derived object only calls ~Derived() then its
    // parent dctor ~Base().
    Derived* pB2 = new Derived2;
    delete pB2;
}

