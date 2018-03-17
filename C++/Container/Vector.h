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
    Vector() :_first(nullptr), _last(nullptr), _end(nullptr)
    {
    }

    Vector(size_t count, const T& value)
    {
        _first = alloc.allocate(count);
        std::uninitialized_fill(_first, _first + count, value);
        _last = _end = _first + count;
    }

    Vector(size_t count)
    {
        _first = alloc.allocate(count);
        std::uninitialized_fill(_first, _first + count, 0);
        _last = _end = _first + count;
    }

    Vector(iterator first, iterator last)
    {
        size_t count = last - first;
        _first = alloc.allocate(count);
        _last = _end = std::uninitialized_copy(first, last, _first);
    }

    // TODO: Vector(const Vector& other)
    Vector(Vector& other) :_first(nullptr), _last(nullptr), _end(nullptr)
    {
        size_t count = other.End() - other.Begin();
        // note other vector could be empty.
        if (count > 0)
        {
            _first = alloc.allocate(count);
            _last = _end = std::uninitialized_copy(other.Begin(), other.End(), _first);
        }
    }

    Vector(Vector&& other)
    {
        // TODO
    }

    ~Vector()
    {
        Destroy(_first, _last);
        alloc.deallocate(_first, Capacity());

        _first = _last = _end = nullptr;
    }

    Vector& operator=(const Vector& other)
    {
        if (&other == this)
            return;

        size_t count = other.End() - other.Begin();
        _first = alloc.allocate(count);
        _last = _end = std::uninitialized_copy(other.Begin(), other.End(), _first);
    }

    Vector& operator=(Vector&& other)
    {
        // TODO
    }

    /*******************************************************/
    // Capacity
    /*******************************************************/
    bool Empty() const
    {
        return _first == _last;
    }

    // Returns the number of elements in the container
    size_t Size() const
    {
        return _last - _first;
    }

    // Returns the number of elements that the container has currently allocated space for.
    size_t Capacity() const
    {
        return _end - _first;
    }

    // Increase the capacity of the vector to a value that's greater or equal to new_cap. 
    // If new_cap is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
    // If new_cap is greater than capacity(), all iterators, including the past-the-end iterator, and all references 
    // to the elements are invalidated. Otherwise, no iterators or references are invalidated.
    void Reserve(size_t newCapacity)
    {
        if (newCapacity <= Capacity())
            return;

        iterator newFirst = alloc.allocate(newCapacity);
        iterator newLast = std::uninitialized_copy(_first, _last, newFirst);

        // destroy old storage by:
        // deconstruct objects from _first to _last,
        // and deallocate storage from _first to _end.
        Destroy(_first, _last);
        alloc.deallocate(_first, Capacity());

        // reset iterators
        _first = newFirst;
        _last = newLast;
        _end = _first + newCapacity;
    }

    /*******************************************************/
    // Modifiers
    /*******************************************************/

    // Assigns values to the container.
    // All iterators, pointers and references to the elements of the container are invalidated. 
    // The past-the-end iterator is also invalidated.(not necessarily correct)
    // Replaces the contents with count copies of value.
    // Note: from std implementation, assign uses insert internally which cause reallocation if 
    // new size()[=count] is greater than old capacity().
    void Assign(size_t count, const T& value)
    {
        Clear();
        Insert(Begin(), count, value);
    }

    // Replaces the contents with copies of those in the range[first, last).
    void Assign(iterator first, iterator last)
    {
        // TODO
    }

    // Removes all elements from the container.
    // Invalidates any references, pointers, or iterators referring to contained elements.
    // Any past-the-end iterators are also invalidated.
    // Leaves the capacity() of the vector unchanged.
    void Clear()
    {
        //for (auto ptr = _first; ptr != _last; ++ptr)
        //{
        //    alloc.destroy(ptr);
        //}
        Destroy(_first, _last);
        _last = _first;
    }

    // inserts value before pos.
    // Causes reallocation if the new size() is greater than the old capacity().
    // If the new size() is greater than capacity(), all iterators and references are invalidated.
    // Otherwise, only the iterators and references before the insertion point remain valid.
    // The past-the-end iterator is also invalidated.
    iterator Insert(iterator pos, const T& value)
    {
        return Insert(pos, 1, value);
    }

    // Note here only implement on version with iterator input not const_iterator.
    // const iterator pos cannot fit uninitialized_copy.
    iterator Insert(iterator pos, size_t count, const T& value)
    {
        size_t offset = pos - _first;
        size_t unusedCapacity = _end - _last;

        if (count == 0)
        {
            ;
        }
        // no need to reallocate
        else if (count <= unusedCapacity)
        {
            // case 1: elements after insert position is greater than count.
            // assume storage is like: 11p220000
            // 0 stands for unused slot, p stands for value at insert position.
            // elements after insertion is the elements needs to move, size is moveSize.
            // assume insert 1 X.
            // 11p220000
            // 11Xp22000
            // 2 overlap for 22 vs p2, overlap size is moveSize-count, un-overlapped size is count.
            // when there is no overlap(moveSize-count<=0), we can directly move.

            // assume insert 2 X
            // 11p220000
            // 11XXp2200
            // to achieve that, we cannot directly move p22, because there is a overlap for last iterator,
            // directly copy p22 will firstly override value in last iterator, so
            // we need to firstly copy the un-overlapped value [last-count,last) to [last,last+count),
            // then copy value at insert pos to [pos, last-count) to [pos+count, last),
            // so right now whole elements needs to be moved is from [pos, last) to [pos+count, last+count),
            // note [last-count,last)+[pos, last-count)=[pos, last),
            // and [last,last+count)+[pos+count, last)=[pos+count, last+count).
            // the slot to insert value can used to fill [pos, pos+count) now.

            // case 2: elements after insert position is equal to count.
            // assume insert 3 X
            // 11p220000
            // 11XXXp220
            // no last iterator overlap, directly copy [pos,last) to [pos+count,last+count),
            // then fill [pos, pos+count) with new values.

            // case 3: elements after insert position is greater to unused capacity.
            // assume insert 4 X
            // 11p220000
            // 11XXXXp22
            // no last iterator overlap. same as case 2.

            size_t moveSize = _last - pos;
            if (moveSize > count)
            {
                iterator newLast = std::uninitialized_copy(_last - count, _last, _last);
                // stl uses copy_backward here for?
                std::uninitialized_copy(pos, _last - count, pos + count);
                std::uninitialized_fill_n(pos, count, value);
                _last = newLast;
            }
            else
            {
                iterator newLast = std::uninitialized_copy(pos, _last, pos + count);
                std::uninitialized_fill_n(pos, count, value);
                _last = newLast;
            }
        }
        else // reallocate
        {
            // try to reallocate 50% growth of storage if available.
            size_t minimalStorage = Size() + count;
            //size_t newCapacity = (3/2)*Capacity(); // note this is incorrect, newCap =(1.5)*Capacity=1*Capacity.
            size_t newCapacity = Capacity()*3/2;
            if (newCapacity < minimalStorage)
            {
                newCapacity = minimalStorage;
            }

            iterator newFirst = alloc.allocate(newCapacity);
            iterator newLast = std::uninitialized_copy(_first, pos, newFirst);
            newLast = std::uninitialized_fill_n(newLast, count, value);
            newLast = std::uninitialized_copy(pos, _last, newLast);

            // destroy old vector
            Destroy(_first, _last);
            alloc.deallocate(_first, Capacity());

            // reset iterator
            _first = newFirst;
            _last = newLast;
            _end = _first + newCapacity;
        }
        // return the iterator of inserted value.
        return Begin() + offset;
    }

    iterator Erase(iterator pos)
    {
        if (pos + 1 != End())
        {
            std::uninitialized_copy(pos + 1, _last, pos);
        }
        Destroy(_last - 1, _last);
        --_last;

        return pos;
    }

    iterator Erase(iterator first, iterator last)
    {
        iterator newLast = std::uninitialized_copy(last, _last, first);
        Destroy(newLast, _last);
        _last = newLast;

        return first;
    }

    void Push_Back(const T& value)
    {
        if (_last != _end)
        {
            std::uninitialized_fill_n(_last, 1, value);
            ++_last;
        }
        else
        {
            // need to reallocate.
            Reserve(Capacity()*3/2);
            Push_Back(value);
        }
    }

    void Pop_back()
    {
        Destroy(_last - 1, _last);
        --_last;
    }

    /*******************************************************/
    // Iterators
    /*******************************************************/

    // specially for "Range for". Need begin(),end().
    iterator begin()
    {
        return _first;
    }
    iterator end()
    {
        return _last;
    }

    iterator Begin()
    {
        return _first;
    }

    const_iterator CBegin()
    {
        return _first;
    }

    iterator End()
    {
        return _last;
    }

    const_iterator CEnd()
    {
        return _last;
    }

    /*******************************************************/
    // Accessor
    /*******************************************************/

private:
    // destroy(deconstruct) objects in range.
    // wrap this function to pass iterator(pointer) as value so don't change _first
    // since we need to deallocate from _first later.
    void Destroy(iterator first, iterator last)
    {
        for (; first != last; ++first)
        {
            alloc.destroy(first);
        }
    }

private:
    std::allocator<T> alloc;

    iterator _first;
    iterator _last;
    iterator _end;
};

// test routines for vector
template<typename T>
void PrintVector(T t)
{
    // iterator test
    // use range to output elements
    for (auto v : t)
    {
        cout << v << " ";
    }
    cout << endl;

    //// use iterator to output elements
    //for (vector<int>::iterator iter = vec2.begin(); iter != vec2.end(); iter++)
    //{
    //    cout << *iter << " ";
    //}
    //cout << endl;
}

void TestSTDVector()
{
    // ctor test
    vector<int> vec1;
    vector<int> vec2(5, 1);
    vector<int> vec3(5);
    //vector<int> vec4{ 1, 2, 3, 4, 5 };
    vector<int> vec5(vec3.begin(), vec3.end());

    PrintVector(vec1);
    PrintVector(vec2);
    PrintVector(vec3);
    PrintVector(vec5);

    // assign/insert test
    // if assigned size is less(equal) to current one, then capacity will not be changed.
    // also first iterator is not changed.
    // if assigned size is larger than current one, first iterator is reallocated.
    vec2.assign(4, 2);
    PrintVector(vec2);

    vec2.assign(6, 3);
    PrintVector(vec2);

    vec2.insert(vec2.begin() + 1, 5);
    PrintVector(vec2);

    vec2.insert(vec2.begin() + 1, 2, 4);
    PrintVector(vec2);

    // capacity test
    vec5.reserve(10); // iterator should be changed.
    vec5.clear();
    size_t size = vec5.size();
    size_t cap = vec5.capacity();

    // element manipulation
    vec5.push_back(6);
    PrintVector(vec5);

    vec5.pop_back();
    PrintVector(vec5);

    vec2.erase(vec2.begin());
    PrintVector(vec2);

    vec2.erase(vec2.begin(), vec2.begin() + 2);
    PrintVector(vec2);

    cout << "end of test std vector." << endl;
}

void TestMyVector()
{
    // ctor test
    Vector<int> vec1;
    Vector<int> vec2(5, 1);
    Vector<int> vec3(5);
    //Vector<int> vec4{ 1, 2, 3, 4, 5 };
    Vector<int> vec5(vec3.Begin(), vec3.End());

    PrintVector(vec1);
    PrintVector(vec2);
    PrintVector(vec3);
    PrintVector(vec5);

    // assign/insert test
    // if assigned size is less(equal) to current one, then capacity will not be changed.
    // also first iterator is not changed.
    // if assigned size is larger than current one, first iterator is reallocated.
    vec2.Assign(4, 2);
    PrintVector(vec2);

    vec2.Assign(6, 3);
    PrintVector(vec2);

    vec2.Insert(vec2.Begin() + 1, 5);
    PrintVector(vec2);

    vec2.Insert(vec2.Begin() + 1, 2, 4);
    PrintVector(vec2);

    // capacity test
    vec5.Reserve(10); // iterator should be changed.
    vec5.Clear();
    size_t size = vec5.Size();
    size_t cap = vec5.Capacity();

    // element manipulation
    vec5.Push_Back(6);
    PrintVector(vec5);

    vec5.Pop_back();
    PrintVector(vec5);

    vec2.Erase(vec2.Begin());
    PrintVector(vec2);

    vec2.Erase(vec2.Begin(), vec2.Begin() + 2);
    PrintVector(vec2);

    cout << "end of test Vector." << endl;
}

void TestVector()
{
    TestSTDVector();
    TestMyVector();
}

#endif

