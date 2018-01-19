//**************************************************************
//         boost::scoped_ptr alike smart pointer
//**************************************************************

#ifndef SCOPEDPTR_H
#define SCOPEDPTR_H

// ScopedPtr is very like UniquePtr, but neither copyable nor movable.
template<typename T>
class ScopedPtr
{
public:
    ScopedPtr(T* p = nullptr) :rawPointer(p)
    {
    }

    ~ScopedPtr()
    {
        delete rawPointer;
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

    // destroy raw pointer and store new one.
    void reset(T* p = nullptr)
    {
        if (p != rawPointer)
        {
            delete rawPointer;
        }
        rawPointer = p;
    }

private:
    ScopedPtr(ScopedPtr& sp) = delete;

    ScopedPtr<T>& operator=(ScopedPtr<T>& sp) = delete;

private:
    T* rawPointer;
};

#endif
