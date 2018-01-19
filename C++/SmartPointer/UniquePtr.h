//**************************************************************
//         std::unique_ptr alike smart pointer
//**************************************************************

#ifndef UNIQUEPTR_H
#define UNIQUEPTR_H

// UniquePtr is like AutoPtr, but does not support copy ctor/operator.
// UniquePtr owns object exclusively, non-copyable but supports ownership transfer(by move).
template<typename T>
class UniquePtr
{
public:
    UniquePtr(T* p = nullptr) :rawPointer(p)
    {
    }

    ~UniquePtr()
    {
        delete rawPointer;
    }

    UniquePtr(UniquePtr&& other)
    {
        reset(other.release());
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

    // return raw pointer and give up ownership.
    T* release()
    {
        T* temp = rawPointer;
        rawPointer = nullptr;
        return temp;
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
    UniquePtr(UniquePtr& sp) = delete;

    UniquePtr<T>& operator=(UniquePtr<T>& sp) = delete;

private:
    T* rawPointer;
};

#endif
