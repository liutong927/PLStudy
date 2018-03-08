//**************************************************************
//         std::vector alike container compatible with STL
//**************************************************************

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class Vector
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    /*******************************************************/
    // ctor and dtor
    /*******************************************************/
    Vector() :first(nullptr), last(nullptr), end(nullptr)
    {
    }

    Vector(size_t count, const T& value)
    {

    }

    Vector(size_t count)
    {

    }

    Vector(iterator first, iterator last)
    {

    }

    Vector(const Vector& other)
    {

    }

    Vector(Vector&& other)
    {

    }

    ~Vector()
    {
    }

    Vector& operator=(const Vector& other)
    {

    }

    Vector& operator=(Vector&& other)
    {

    }

    /*******************************************************/
    // Capacity
    /*******************************************************/
    bool Empty() const
    {

    }

    // Returns the number of elements in the container
    size_t Size() const
    {

    }

    // Increase the capacity of the vector to a value that's greater or equal to new_cap. 
    // If new_cap is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
    void Reserve(size_t newCapacity)
    {

    }

    // Returns the number of elements that the container has currently allocated space for.
    size_t Capacity() const
    {

    }

    /*******************************************************/
    // Modifiers
    /*******************************************************/
    void Clear()
    {

    }

    void Insert(iterator pos, const T& value)
    {

    }

    iterator Erase(iterator pos)
    {

    }

    iterator Erase(iterator first, iterator last)
    {

    }

    void Push_Back(const T& value)
    {

    }

    void Pop_back()
    {

    }

    /*******************************************************/
    // Iterators
    /*******************************************************/
    iterator Begin()
    {

    }

    const_iterator CBegin()
    {

    }

    iterator End()
    {

    }

    const_iterator CEnd()
    {

    }
private:
    std::allocator<T> alloc;

    iterator first;
    iterator last;
    iterator end;
};

void TestSTDVector()
{
    vector<int> emptyVec;

    vector<int> vec = { 1, 2, 3, 4, 5 };
    vec.push_back(6);

    // use range to output elements
    for (auto v : vec)
    {
        cout << v << " ";
    }
    cout << endl;

    // use iterator to output elements
    for (vector<int>::iterator iter = vec.begin(); iter != vec.end(); iter++)
    {
        cout << *iter << " ";
    }
    cout << endl;
}

void TestVector()
{
    Vector<int> vec;
}

#endif

