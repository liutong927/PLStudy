//**************************************************************
//         std::weak_ptr alike smart pointer
//**************************************************************

#ifndef WEAKPTR_H
#define WEAKPTR_H

#include "SharedPtr.h"

// non-owning observer to a shared_ptr-managed object that can be promoted temporarily to shared_ptr.

template<typename T>
class WeakPtr
{
public:
    WeakPtr()
    {
    }

    ~WeakPtr()
    {
        if (pRef != nullptr)
        {
            if (--pRef->referenceCount == 0)
            {
                delete pRef;
                pRef = nullptr;
            }
        }
    }

    // copy ctor from another WeakPtr.
    WeakPtr(WeakPtr& sp) :pRef(sp.pRef)
    {
        pRef->referenceCount++;
    }

    WeakPtr<T>& operator=(WeakPtr<T>& sp)
    {
        // check for self-assignment.
        if (this == &sp)
        {
            return *this;
        }

        pRef = sp.pRef;
        pRef->referenceCount++;
        return *this;
    }

    // returns the number of SharedPtr objects that manage the object
    long use_count()
    {

    }

    // checks whether the referenced object was already deleted.
    bool expired()
    {

    }

    // creates a SharedPtr that manages the referenced object.
    SharedPtr<T> lock()
    {

    }

    // releases the ownership of the managed object, if any. 
    void reset()
    {
        pRef->referenceCount--;
        // make this shared ptr own nothing.
        pRef = nullptr;
        // should we handle raw pointer here? No. raw pointer's destruction relies on whether any managing shared ptr own it.
        // one shared ptr reset does not there is no more other shared ptr own it.
    }

private:
    struct Reference
    {
        T* rawPointer;
        int referenceCount;

        Reference(T* p) :rawPointer(p), referenceCount(1)
        {
        };

        ~Reference()
        {
            delete rawPointer;
            rawPointer = nullptr;
        };
    };

    Reference* pRef; // hold the resource and its reference count.
};

#endif
