//**************************************************************
//         std::optional
//**************************************************************

#ifndef OPTIONAL_H
#define OPTIONAL_H

// Optional manages an optional contained value, may or may not be present.
// when to use Optional:
// 1. It is recommended to use optional<T> in situations where there is exactly one, 
//    clear (to all parties) reason for having no value of type T, and where the lack of value
//    is as natural as having any regular value of T.
// 2. Another typical situation is to indicate that we do not have a value yet, but we expect to have it later.
//    This notion can be used in implementing solutions like lazy initialization or a two-phase initialization.

template<typename T>
class Optional
{
public:
    Optional()
    {

    }

    Optional(Optional& other)
    {

    }

    Optional(Optional&& other)
    {

    }

    ~Optional()
    {

    }

    Optional& operator=(Optional& other)
    {

    }

    T* operator->()
    {

    }

    T& operator*()
    {

    }

    operator bool()
    {

    }

    bool HasValue()
    {

    }

    // returns the contained value.
    T& Value()
    {

    }

    // returns the contained value if available, another value otherwise.
    T& ValueOr()
    {

    }

    // destroys any contained value.
    void Reset()
    {

    }

    // Initializes the contained value by direct-initializing (but not direct-list-initializing)
    // with std::forward<Args>(args)... as parameters.
    template< class... Args >
    T& Emplace(Args&&... args)
    {

    }

private:

};

#endif
