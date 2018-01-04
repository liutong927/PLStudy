//**************************************************************
//         std::shared_ptr alike smart pointer
//**************************************************************

#ifndef SHAREDPTR_H
#define SHAREDPTR_H

template<typename T>
class SharedPtr
{
public:
    SharedPtr(T* p = nullptr) :rawPointer(p)
    {
    }

    ~SharedPtr()
    {
        delete rawPointer;
    }

    SharedPtr(SharedPtr& sp) :rawPointer(sp.release())
    {
    }

    SharedPtr<T>& operator=(SharedPtr<T>& sp)
    {
        reset(sp.release());

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
    T* rawPointer;
};

#endif
