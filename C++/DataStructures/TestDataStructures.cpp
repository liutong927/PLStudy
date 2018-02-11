//**************************************************************
//         Test routine for data structures
//**************************************************************

#include "Optional.h"
#include <boost/optional/optional.hpp>
#include <iostream>

class Test
{
public:
    Test() :a(0), b(0)
    {
    }

    Test(int a, int b) :a(a), b(b)
    {
    }

    void Print()
    {
        cout << "a=" << a << ", b=" << b << endl;
    }

private:
    int a;
    int b;
};

int main()
{
    Optional<string> a("ok");
    Optional<string> b;
    boost::optional<string> c;

    cout << *a << endl;
    cout << a.Value() << endl;

    if (!b.HasValue())
        cout << "b is not initialized." << endl;
    cout << b.ValueOr(string("none")) << endl;

    Optional<Test> test;
    test.Emplace(1, 2);
    test->Print();
}

