//**************************************************************
//         std::optional
//**************************************************************

#ifndef OPTIONAL_H
#define OPTIONAL_H

// Optional is a container that either has a value of T or nothing.
// Optional manages an optional contained value, may or may not be present.

// In C++ there is no formal notion of uninitialized objects, which means that objects always have an initial value
// even if indeterminate. this has a drawback because you need additional information to tell if an object has been
// effectively initialized. One of the typical ways in which this has been historically dealt with is via a special
// value: EOF, npos, -1, etc... 

// optional<T> intends to formalize the notion of initialization (or lack of it) allowing a program to test whether
// an object has been initialized and stating that access to the value of an uninitialized object is undefined behavior.
// That is, when a variable is declared as optional<T> and no initial value is given, the variable is formally 
// uninitialized. A formally uninitialized optional object has conceptually no value at all and this situation can be
// tested at runtime. It is formally undefined behavior to try to access the value of an uninitialized optional. 
// An uninitialized optional can be assigned a value, in which case its initialization state changes to initialized.
// Furthermore, given the formal treatment of initialization states in optional objects, it is even possible to reset
// an optional to uninitialized.

// when to use Optional:
// 1. It is recommended to use optional<T> in situations where there is exactly one, 
//    clear (to all parties) reason for having no value of type T, and where the lack of value
//    is as natural as having any regular value of T.
// 2. Another typical situation is to indicate that we do not have a value yet, but we expect to have it later.
//    This notion can be used in implementing solutions like lazy initialization or a two-phase initialization.

#include <type_traits>
using namespace std;

template<typename T>
class Optional
{
    // type of T with aligned storage.
    using dataType = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;

public:
    Optional() :m_isInitialized(false)
    {
    }

    Optional(const T& v)
    {
        Construct(v);
    }

    Optional(Optional& other) :m_isInitialized(false)
    {
        if (other.m_isInitialized)
        {
            Construct(other);
        }
    }

    Optional(Optional&& other) :m_isInitialized(false)
    {
        if (other.m_isInitialized)
        {
            Construct(std::move(other));
        }
    }

    ~Optional()
    {
        Destroy();
    }

    Optional& operator=(Optional& other)
    {
        if (m_isInitialized)
        {
            if (other.m_isInitialized)
            {
                // is this correct?
                m_storage = other.m_storage;
            }
            else
            {
                Destroy();
            }
        }
        else
        {
            if (other.m_isInitialized)
            {
                Construct(other);
            }
        }
    }

    // accesses the contained value
    T* operator->()
    {
        return &operator*();
    }

    T& operator*()
    {
        if (m_isInitialized)
            return *((T*)(&m_storage));

        throw std::logic_error("try to get data in a Optional which is not initialized.");
    }

    // checks whether the object contains a value
    operator bool()
    {
        return m_isInitialized;
    }

    bool HasValue()
    {
        return m_isInitialized;
    }

    // returns the contained value.
    T& Value()
    {
        if (m_isInitialized)
            return m_storage;
        else
            throw std::logic_error("try to get data in a Optional which is not initialized.");
    }

    // returns the contained value if available, another value otherwise.
    template <class U>
    T& ValueOr(U& v)
    {
        if (m_isInitialized)
            return m_storage;
        else
            return v;
    }

    // destroys any contained value.
    void Reset()
    {
        Destroy();
    }

    // constructs the contained value in-place.
    template<class... Args>
    void Emplace(Args&&... args)
    {
        Destroy();
        Construct(std::forward<Args>(args)...);
    }

private:
    template<class... Args>
    void Construct(Args&&... args)
    {
        // placement new T
        new (&m_storage) T(std::forward<Args>(args)...);
        m_isInitialized = true;
    }

    // destroy current value, leave it as uninitialized.
    void Destroy()
    {
        if (m_isInitialized)
        {
            ((T*)&m_storage)->~T();
            m_isInitialized = false;
        }
    }

private:
    dataType m_storage;
    bool m_isInitialized;
};

#endif
