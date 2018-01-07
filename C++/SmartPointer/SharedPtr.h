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


#endif
