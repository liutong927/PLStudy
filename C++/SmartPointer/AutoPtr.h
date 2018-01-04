//**************************************************************
//         std::auto_ptr alike smart pointer
//**************************************************************

#ifndef AUTOPTR_H
#define AUTOPTR_H

template<typename T>
class AutoPtr
{
public:
    AutoPtr(T* p = nullptr) :rawPointer(p)
    {
    }

    ~AutoPtr()
    {
        delete rawPointer;
    }

    AutoPtr(AutoPtr& sp) :rawPointer(sp.release())
    {
    }

    AutoPtr<T>& operator=(AutoPtr<T>& sp)
    {
        // why need to reset here??
        // transfer ownership to left side object.
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
