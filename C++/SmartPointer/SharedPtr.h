//**************************************************************
//         std::shared_ptr alike smart pointer
//**************************************************************

#ifndef SHAREDPTR_H
#define SHAREDPTR_H

// multiple SharedPtr can share/own same pointer which avoid redundant storage
// of same resource, destroy pointer when there is not any SharedPtr own it.
// reference counting is used to count the owning number.

// SharedPtr objects can only share ownership by copying their value.
// if two SharedPtr are constructed from same pointer, they are NOT sharing.

// Attempt 1:
// failed: copying shared ptr will increase its reference count, but not copied shared ptr.
/*
template<typename T>
class SharedPtr
{
public:
    SharedPtr(T* p = nullptr) :rawPointer(p)
    {
        referenceCount = 1;
    }

    ~SharedPtr()
    {
        if (referenceCount==0)
            delete rawPointer;
    }

    SharedPtr(SharedPtr& sp) :rawPointer(sp.get())
    {
        referenceCount++;
    }

    SharedPtr<T>& operator=(SharedPtr<T>& sp)
    {
        reset(sp.get());
        referenceCount++;

        return *this;
    }

    T& operator*()
    {
        if (rawPointer == nullptr)
            throw rawPointer;
        return *rawPointer;
    }

    T* operator->()
    {
        if (rawPointer == nullptr)
            throw rawPointer;
        return rawPointer;
    }

    // return raw pointer.
    T* get() const
    {
        return rawPointer;
    }

    // destroy raw pointer, make managing sharedptr object empty.
    void reset()
    {
        if (rawPointer != nullptr)
        {
            delete rawPointer;
        }
    }

private:
    T* rawPointer;
    int referenceCount;
};
*/

// Attempt 2:
// failed: copying and copied shared ptr will both increase its reference count.
// but not for other shared ptr, e.g, SharedPtr A, B, C, B copy A, C copy B.
// only B and C update its reference count correctly, see copy ctor.

// so it is not right to put referenceCount directly on the field of SharedPtr.
// Need to have an object which contain reference count and raw pointer, and make
// SharedPtr own the pointer to this object. This will make all sharedPtr own the same
// referenceCount object which leads to Attempt 3.
/*
template<typename T>
class SharedPtr
{
public:
    SharedPtr(T* p = nullptr) :rawPointer(p)
    {
        referenceCount = 1;
    }

    ~SharedPtr()
    {
        if (referenceCount == 0)
            delete rawPointer;
    }

    SharedPtr(SharedPtr& sp) :rawPointer(sp.get())
    {
        // need to update reference count of sp and this shared ptr.
        sp.referenceCount++;
        referenceCount = sp.referenceCount;
    }

    SharedPtr<T>& operator=(SharedPtr<T>& sp)
    {
        rawPointer = sp.get();
        sp.referenceCount++;
        referenceCount = sp.referenceCount;

        return *this;
    }

    T& operator*()
    {
        if (rawPointer == nullptr)
            throw rawPointer;
        return *rawPointer;
    }

    T* operator->()
    {
        if (rawPointer == nullptr)
            throw rawPointer;
        return rawPointer;
    }

    // return raw pointer.
    T* get() const
    {
        return rawPointer;
    }

    // destroy raw pointer, make managing sharedptr object empty.
    void reset()
    {
        if (rawPointer != nullptr)
        {
            delete rawPointer;
        }
    }

private:
    T* rawPointer;
    int referenceCount;
};
*/

// Attempt 3:

template<typename T>
class SharedPtr
{
public:
    SharedPtr(T* p = nullptr)
    {
        pRef = new Reference(p);
    }

    ~SharedPtr()
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

    // make copying SharedPtr point to same resource.
    SharedPtr(SharedPtr& sp) :pRef(sp.pRef)
    {
        pRef->referenceCount++;
    }

    SharedPtr<T>& operator=(SharedPtr<T>& sp)
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

    T& operator*()
    {
        T* raw = pRef->rawPointer;

        if (raw == nullptr)
            throw raw;
        return *raw;
    }

    T* operator->()
    {
        T* raw = pRef->rawPointer;

        if (raw == nullptr)
            throw raw;
        return raw;
    }

    // return raw pointer.
    T* get() const
    {
        return pRef->rawPointer;
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
