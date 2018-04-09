//**************************************************************
//         std::allocator implementation
//**************************************************************
#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>

using namespace std;

template<typename T>
class Allocator
{
public:
    // typedef
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    // convert allocator<T> to allocator<U>.
    template<typename U>
    struct rebind
    {
        typedef Allocator<U> other;
    };

    // ctor & dctor
    Allocator()
    {
    }

    ~Allocator()
    {
    }

    Allocator(const Allocator&)
    {
    }

    template<typename U>
    Allocator(const Allocator<U>&)
    {
    }

    // address
    pointer address(reference ref)
    {
        return &ref;
    }

    const_pointer address(const_reference ref)
    {
        return &ref;
    }

    // memory allocation

    //allocates storage suitable for n objects of type T, but does not construct them
    pointer allocate(size_type count)
    {
        void* ptr = nullptr;
        // previous typo error "if(count=0)" cause count=0, which leads to heap corruption.
        if (count == 0)
            ;
        else if (((size_t(-1) / sizeof(T)) < count) || (ptr = ::operator new(count*sizeof(T))) == nullptr)
            throw std::out_of_range("bad allocation.");

        return (T*)ptr;
    }

    // deallocates storage pointed to ptr, which must be a value returned by a previous call to allocate 
    // that has not been invalidated by an intervening call to deallocate.
    // n must match the value previously passed to allocate.Does not throw exceptions.
    void deallocate(pointer ptr, size_type count)
    {
        ::operator delete(ptr);
    }

    // construction/deconstruction
    void construct(pointer ptr)
    {
        ::new(ptr) T();
    }

    void construct(pointer ptr, const_reference ref)
    {
        ::new(ptr) T(ref);
    }

    void destroy(pointer ptr)
    {
        ptr->~T();
    }

    // size
    size_type max_size()
    {
        return (size_t(-1) / sizeof(T));
    }
private:
};

void TestAllocator()
{
    Allocator<int> alloc;
    auto ptr = alloc.allocate(5);
    alloc.construct(ptr, 1);

    cout << *ptr << endl;
    cout << *(ptr+1) << endl;
}

#endif
