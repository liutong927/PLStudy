//**************************************************************
//         std::weak_ptr alike smart pointer
//**************************************************************

#ifndef WEAKPTR_H
#define WEAKPTR_H

#include "BasePtr.h"
#include "SharedPtr.h"

template<typename T>
class SharedPtr;

// non-owning observer to a shared_ptr-managed object that can be promoted temporarily to shared_ptr.
// weak ptr cannot obtain managed resource directly(not overload * and -> operator), has to promote
// to shared ptr to obtain resource.

// Attempt 1:
// need to inherit from BasePtr to use common reference object.
/*
template<typename T>
class WeakPtr
{
public:
    WeakPtr()
    {
    }

    ~WeakPtr()
    {
    }

    template<class T> WeakPtr(SharedPtr<T>& sp)
    {
        pRef->weakRefCount++;
    }

    // copy ctor from another WeakPtr. weak ptr can only contruct from shared ptr,
    // here copied weak ptr will be promoted to shared ptr then call copy ctor.
    WeakPtr(WeakPtr& sp) :pRef(sp.pRef)
    {
        pRef->weakRefCount++;
    }

    WeakPtr<T>& operator=(WeakPtr<T>& sp)
    {
        // check for self-assignment.
        if (this == &sp)
        {
            return *this;
        }

        pRef = sp.pRef;
        pRef->weakRefCount++; // add weak ref count.
        return *this;
    }

    // returns the number of SharedPtr objects that manage the object
    long use_count()
    {
        return pRef->strongRefCount;
    }

    // checks whether the referenced object was already deleted.
    bool expired()
    {
        return pRef->strongRefCount == 0;
    }

    // creates a SharedPtr that manages the referenced object.
    SharedPtr<T> lock()
    {
        return SharedPtr<T>(*this);
    }

    // releases the ownership of the managed object, if any. 
    void reset()
    {
        pRef->weakRefCount--;
        // make this weak ptr own nothing.
        pRef = nullptr;
    }

private:
    struct Reference
    {
        T* rawPointer;
        int strongRefCount;
        int weakRefCount;

        Reference(T* p) :rawPointer(p), strongRefCount(1), weakRefCount(0)
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
*/

// Attempt 2:
// use BasePtr for WeakPtr.
template<typename T>
class WeakPtr: public BasePtr<T>
{
public:
    WeakPtr()
    {
    }

    ~WeakPtr()
    {
    }

    WeakPtr(SharedPtr<T>& sp)
    {
        this->_ResetW(sp);
    }

    // copy ctor from another WeakPtr. weak ptr can only contruct from shared ptr,
    // here copied weak ptr will be promoted to shared ptr then call copy ctor.
    WeakPtr(WeakPtr& sp)
    {
        this->_ResetW(sp);
    }

    WeakPtr<T>& operator=(WeakPtr<T>& sp)
    {
        // check for self-assignment.
        if (this == &sp)
        {
            return *this;
        }

        this->_ResetW(sp);
        return *this;
    }

    WeakPtr<T>& operator=(SharedPtr<T>& sp)
    {
        this->_ResetW(sp);
        return *this;
    }

    // checks whether the referenced object was already deleted.
    bool Expired()
    {
        return UseCount() == 0;
    }

    // creates a SharedPtr that manages the referenced object.
    SharedPtr<T> Lock()
    {
        return SharedPtr<T>(*this);
    }

    // releases the ownership of the managed object, if any. 
    void Reset()
    {
        this->_ResetW();
    }

private:
};

#endif
