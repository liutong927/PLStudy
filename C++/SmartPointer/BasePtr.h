//**************************************************************
//         base ptr class for SharedPtr and WeakPtr
//**************************************************************

#ifndef BASEPTR_H
#define BASEPTR_H

template<typename T>
class BasePtr
{
public:
    BasePtr(T* p = nullptr)
    {
        pRef = new Reference(p);
    }

    ~BasePtr()
    {
        if (pRef != nullptr)
        {
            if (--pRef->strongRefCount == 0)
            {
                delete pRef;
                pRef = nullptr;
            }
        }
    }

    // make copying another BasePtr point to same resource.
    BasePtr(BasePtr& sp) :pRef(sp.pRef)
    {
        pRef->strongRefCount++;
    }

    BasePtr<T>& operator=(BasePtr<T>& sp)
    {
        // check for self-assignment.
        if (this == &sp)
        {
            return *this;
        }

        pRef = sp.pRef;
        pRef->strongRefCount++;
        return *this;
    }

    // return raw pointer.
    T* GetRaw() const
    {
        return pRef->rawPointer;
    }

    int UseCount()
    {
        return pRef->strongRefCount;
    }

    void IncreaseRef()
    {
        pRef->strongRefCount++;
    }

    void IncreaseWeakRef()
    {
        pRef->weakRefCount++;
    }

    void DecreaseWeakRef()
    {
        pRef->weakRefCount--;
    }

    // releases the ownership of the managed object, if any. 
    void _Reset()
    {
        pRef->strongRefCount--;
        // make this shared ptr own nothing.
        pRef = nullptr;
        // should we handle raw pointer here? No. raw pointer's destruction relies on whether any managing shared ptr own it.
        // one shared ptr reset does not there is no more other shared ptr own it.
    }

    void _Reset(BasePtr& other)
    {
        pRef = other.pRef;
        IncreaseRef();
    }

    void _ResetW(BasePtr& other)
    {
        pRef = other.pRef;
        IncreaseWeakRef();
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

#endif
