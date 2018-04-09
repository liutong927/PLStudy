//**************************************************************
//         std::list alike container compatible with STL
//**************************************************************

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <list>
#include "..\Memory\Allocator.h"

using namespace std;

template<typename T>
struct Node
{
    Node* prev;
    Node* next;
    T value;
};

// list iterator cannot use raw pointer like vector.
// act as smart pointer(wrapper for raw pointer) to provide raw data access.
template<typename T>
class list_iterator
{
    typedef Node<T>* NodePtr;
    typedef T& reference; // raw data reference
    typedef T* pointer; // raw data pointer
public:
    list_iterator()
    {
    }

    // construct list iterator from node pointer
    list_iterator(NodePtr np) : nodePtr(np)
    {
    }

    list_iterator(const list_iterator& other) : nodePtr(other.nodePtr)
    {
    }

    bool operator==(const list_iterator& other)
    {
        return nodePtr == other.nodePtr;
    }

    bool operator!=(const list_iterator& other)
    {
        return nodePtr != other.nodePtr;
    }

    reference operator*() const
    {
        return nodePtr->value;
    }

    pointer operator->() const
    {
        return &(nodePtr->value);
    }

    // pre-increment
    list_iterator& operator++()
    {
        nodePtr = nodePtr->next;
        return *this;
    }

    // post-increment
    list_iterator operator++(int)
    {
        list_iterator temp = *this;
        ++(*this);
        return temp;
    }

    // pre-decrement
    list_iterator& operator--()
    {
        nodePtr = nodePtr->prev;
        return *this;
    }

    // post-decrement
    list_iterator operator--(int)
    {
        list_iterator temp = *this;
        --(*this);
        return temp;
    }
public:
    NodePtr nodePtr;// it is iterator for node pointer, make it public for List<T>
};

template<typename T>
class List
{
    typedef Node<T>* NodePtr;
    typedef list_iterator<T> iterator;
    typedef const list_iterator<T> const_iterator;
    typedef T& reference; // raw data reference
    typedef T* pointer; // raw data pointer
public:
    /*******************************************************/
    // ctor and dtor
    /*******************************************************/
    List()
    {
        InitializeList();
    }

    List(size_t count, const T& value)
    {
        InitializeList();
        InsertNodes(Begin(), count, value);
    }

    List(size_t count)
    {
    }

    List(iterator first, iterator last)
    {
    }

    List(const List& other)
    {
    }

    List(List&& other)
    {
    }

    List(std::initializer_list<T> init)
    {
        // TODO
    }

    ~List()
    {
    }

    List& operator=(const List& other)
    {
        if (&other == this)
            return;

    }

    List& operator=(List&& other)
    {
    }

    /*******************************************************/
    // Capacity
    /*******************************************************/
    bool Empty() const
    {
        return size == 0;
    }

    // Returns the number of elements in the container
    size_t Size() const
    {
        return size;
    }

    /*******************************************************/
    // Modifiers
    /*******************************************************/

    // Assigns values to the container.
    void Assign(size_t count, const T& value)
    {
    }

    // Replaces the contents with copies of those in the range[first, last).
    void Assign(iterator first, iterator last)
    {
    }

    // Removes all elements from the container.
    void Clear()
    {
    }

    // inserts value before pos.
    iterator Insert(iterator pos, const T& value)
    {
        // create new list node and specify its prev/next.
        NodePtr tmp = CreateNode(value);
        tmp->next = pos.nodePtr;
        tmp->prev = pos.nodePtr->prev;
        // reset original pos and pos-1
        pos.nodePtr->prev = tmp;
        pos.nodePtr->prev->next = tmp;
        return tmp;
    }

    iterator Insert(iterator pos, size_t count, const T& value)
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

    void Pop_Back()
    {
    }

    void Push_Front(const T& value)
    {
    }

    void Pop_Front()
    {
    }

    // constructs element in-place.
    template<class... Args>
    iterator Emplace(iterator pos, Args&&... args)
    {
        // TODO
    }

    // constructs an element in-place at the beginning.
    template<class... Args>
    void Emplace_Front(Args&&... args)
    {
        // TODO
    }

    // constructs an element in-place at the end.
    template<class... Args>
    void Emplace_Back(Args&&... args)
    {
        // TODO
    }

    /*******************************************************/
    // Iterators
    /*******************************************************/

    // specially for "Range for". Need begin(),end().
    iterator begin()
    {
        return Begin();
    }

    iterator end()
    {
        return End();
    }

    iterator Begin()
    {
        return head->next;
    }

    const_iterator CBegin()
    {
        return Begin();
    }

    iterator End()
    {
        return head;
    }

    const_iterator CEnd()
    {
        return End();
    }

    /*******************************************************/
    // Accessor
    /*******************************************************/

    // Returns a reference to the first element in the container.
    // Calling front on an empty container is undefined.
    reference Front()
    {
        return *Begin();
    }

    // Returns reference to the last element in the container.
    // Calling back on an empty container is undefined.
    reference Back()
    {
        return *(End() - 1);
    }

    /*******************************************************/
    // Operations
    /*******************************************************/

private:

    /*******************************************************/
    // Allocator for one node of list
    /*******************************************************/

    // allocate one node
    // given list<int>, we have allocator<T>, but we actually need to
    // allocate memory Node<int>, not int.
    NodePtr AllocNode()
    {
        // conversion of allocator from T to Node<T>
        typename allocator<T>::rebind<Node<T>>::other allocProxy;
        return allocProxy.allocate(1);
    }

    void DeallocNode(NodePtr np)
    {
        typename allocator<T>::rebind<Node<T>>::other allocProxy;
        allocProxy.deallocate(np);
    }

    NodePtr CreateNode(const T& t)
    {
        NodePtr np = AllocNode();
        // use allocator<T> to construct.
        alloc.construct(&np->value, t);
        return np;
    }

    void DestroyNode(NodePtr np)
    {
        alloc.destroy(&np->value);
        DeallocNode(np);
    }

    /*******************************************************/
    // Utility functions to handle node
    /*******************************************************/

    // initialize list by allocate a node, make its next/prev points to itself.
    void InitializeList()
    {
        head = AllocNode();
        head->next = head;
        head->prev = head;
    }

    // insert one node into list at position
    void InsertNode(iterator pos, const T& value)
    {
        // create new list node and specify its prev/next.
        NodePtr tmp = CreateNode(value);
        tmp->next = pos.nodePtr;
        tmp->prev = pos.nodePtr->prev;
        // reset original pos and pos-1
        //pos.nodePtr->prev = tmp;
        //pos.nodePtr->prev->next = tmp;
        // note we must first change pos-1's next and then pos-1
        pos.nodePtr->prev->next = tmp;
        pos.nodePtr->prev = tmp;

        // update size
        IncreaseSize(1);
    }

    // insert n nodes into list at position
    void InsertNodes(iterator pos, size_t count, const T& value)
    {
        for (; count > 0; --count)
        {
            InsertNode(pos, value);
        }
    }

    void IncreaseSize(size_t count)
    {
        size += count;
    }

private:
    std::allocator<T> alloc;
    NodePtr head;// head node of list, make list meets the stl [) range.
    size_t size;// number of elements.
};

void TestSTDList()
{
    list<int> lst1;
    list<int> lst2(5, 1);
}

void TestMyList()
{
    List<int> lst1;
    List<int> lst2(5, 1);
}

void TestList()
{
    TestSTDList();
    TestMyList();
}

#endif

