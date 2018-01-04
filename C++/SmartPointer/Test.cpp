//**************************************************************
//         std::auto_ptr alike smart pointer
//**************************************************************

#include "AutoPtr.h"
#include <memory>

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

    int* pp = new int(5);
    std::auto_ptr<int> ap(pp);
    std::auto_ptr<int> apc(ap);
    std::auto_ptr<int> apcc = apc;
}

int main()
{
    AutoPtr_Test();
}

