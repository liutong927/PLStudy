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
        InitializeList();
        InsertNodes(Begin(), count, 0);
    }

    List(iterator first, iterator last)
    {
        InitializeList();
        InsertRange(Begin(), first, last);
    }

    List(const List& other)
    {
        InitializeList();
        InsertRange(Begin(), other.Begin(), other.End());
    }

    List(List&& other)
    {
        InitializeList();
        std::swap(head, other.head);
        std::swap(size, other.size);
    }

    List(std::initializer_list<T> init)
    {
        // TODO
    }

    ~List()
    {
        Clear();
    }

    List& operator=(const List& other)
    {
        if (this != &other)
        {
            Assign(other.Begin(), other.End());
        }

        return *this;
    }

    List& operator=(List&& other)
    {
        if (this != &other)
        {
            Clear();
            std::swap(head, other.head);
            std::swap(size, other.size);
        }

        return *this;
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
        Clear();
        InsertNodes(Begin(), count, value);
    }

    // Replaces the contents with copies of those in the range[first, last).
    void Assign(iterator first, iterator last)
    {
        // NOTE: note std assign range will implement by reusing old storage
        // and erase extra nodes to improve performance. Here do it for simplicity
        // by clear old storage directly and insert new ones.
        Clear();
        InsertRange(Begin(), first, last);
    }

    // Removes all elements from the container.
    void Clear()
    {
        // delete from begin() node
        NodePtr currentNode = head->next;
        while (currentNode != head)
        {
            // cache next node of current, we will delete current now so we cannot get its next after deletion.
            NodePtr nextNode = currentNode->next;
            DestroyNode(currentNode);
            currentNode = nextNode;
        }

        // reset head
        head->next = head;
        head->prev = head;
        size = 0;
    }

    // inserts value before pos.
    iterator Insert(iterator pos, const T& value)
    {
        InsertNode(pos, value);
        // we can directly return pos-1 since insert will not invalidate pos iterator.
        return --pos;
    }

    // return iterator pointing to the first element inserted, or pos if count==0.
    iterator Insert(iterator pos, size_t count, const T& value)
    {
        // NOTE: should we check if it is inserted into begin or not??
        // below logic is enough, although std implementation makes separate logic to deal with insert at begin.
        iterator prevPos = pos;
        --prevPos; // pos's prev iterator.
        InsertNodes(pos, count, value);
        return ++prevPos;// after insertion, prevPos next node is the first inserted node.
    }

    // Removes the element at pos.
    iterator Erase(iterator pos)
    {
        // unlink: reset pos's prev node and next node
        NodePtr prevNode = pos.nodePtr->prev;
        NodePtr nextNode = pos.nodePtr->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        DecreaseSize(1);
        DestroyNode(pos.nodePtr);
        return nextNode;
    }

    // Removes the elements in the range[first; last).
    // Return iterator following the last removed element. 
    // If the iterator pos refers to the last element, the end() iterator is returned.
    iterator Erase(iterator first, iterator last)
    {
        //// unlink
        //NodePtr prevNode = *first->prev;
        //NodePtr nextNode = *last->next;
        //prevNode->next = nextNode;
        //nextNode->prev = prevNode;

        //for (NodePtr current = first; current != last; ++current)
        //{
        //    DestroyNode(current);
        //    DecreaseSize(1);
        //}
        //return nextNode;

        // remove all, keep this branch as std for performance, although else branch can erase all nodes too.
        if (first == Begin() && last == End())
        {
            Clear();
            return End();
        }
        else// remove range
        {
            while (first != last)
            {
                first = Erase(first);
            }
            return last;
        }
    }

    void Push_Back(const T& value)
    {
        Insert(End(), value);
    }

    void Pop_Back()
    {
        Erase(--End());
    }

    void Push_Front(const T& value)
    {
        Insert(Begin(), value);
    }

    void Pop_Front()
    {
        Erase(Begin());
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

    iterator Begin() const
    {
        return head->next;
    }

    const_iterator CBegin()
    {
        return Begin();
    }

    const_iterator CBegin() const
    {
        return Begin();
    }

    iterator End()
    {
        return head;
    }

    iterator End() const
    {
        return head;
    }

    const_iterator CEnd()
    {
        return End();
    }

    const_iterator CEnd() const
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
        // use pre-decrement in case we do not overload operator-.
        return *(--End());
    }

    /*******************************************************/
    // Operations
    /*******************************************************/

    // merges two sorted lists. The lists should be sorted into ascending order.
    // No elements are copied. The container other becomes empty after the operation.
    void Merge(List& other)
    {
        iterator first1 = Begin();
        iterator last1 = End();
        iterator first2 = other.Begin();
        iterator last2 = other.End();

        while (first1 != last1 && first2 != last2)
        {
            if (*first1 > *first2)
            {
                iterator next = first2;
                Transfer(first1, first2, ++next);// transfer first2 to list1, [first1, first1+1)
                first2 = next;
            }
            else
            {
                ++first1;
            }
        }

        // after loop the whole list1, if still left with list2, transfer left nodes to append to list1
        if (first2 != last2)
        {
            Transfer(first1, first2, last2);
        }
    }

    // transfer all elements from another list into *this.
    // The elements are inserted before the element pointed to by pos.
    // The container other becomes empty after the operation.
    void Splice(iterator pos, List& other)
    {
        if (!other.Empty())
        {
            Transfer(pos, other.Begin(), other.End());
        }
    }

    // Transfers the element pointed to by it from other into *this.
    // The element is inserted before the element pointed to by pos.
    void Splice(iterator pos, List& other, iterator it)
    {
        iterator last = it;
        ++last;
        Transfer(pos, it, last);
    }

    // Transfers the elements in the range [first, last) from other into *this.
    // The elements are inserted before the element pointed to by pos.
    void Splice(iterator pos, List& other, iterator first, iterator last)
    {
        Transfer(pos, first, last);
    }

    // removes all elements that are equal to value
    void Remove(const T& value)
    {
        // NOTE: A typical code error, current iterator will be invalid after erase,
        // thus ++current will get a wrong memory location.
        //for (iterator current = Begin(); current != End(); ++current)
        //{
        //    if (*current == value)
        //    {
        //        Erase(current);
        //    }
        //}

        iterator first = Begin();
        iterator last = End();
        while (first != last)
        {
            iterator next = first;
            ++next; // get next node before do Erase.
            if (*first == value)
            {
                Erase(first);
            }
            first = next;
        }
    }

    // Reverses the order of the elements in the container. No references or iterators become invalidated.
    void Reverse()
    {
        iterator first = Begin();
        ++first;// start from 2nd node

        while (first != End())
        {
            iterator old = first;
            Transfer(Begin(), old, ++first);//move each node before begin()==head's next node which means
                                            // move each node at the front of this list.
        }
    }

    // Removes all consecutive duplicate elements from the container.
    // Only the first element in each group of equal elements is left.
    void Unique()
    {
        iterator first = Begin();
        iterator last = End();
        while (first != last)
        {
            iterator next = first;
            ++next;
            if (*first == *next)
            {
                // note first iterator is not changed/erased if it is equal to next.
                // we just remove next, then continue with the loop.
               Erase(next);
            }
            else
            {
                first = next;
            }
        }
    }

    // Sorts the elements in ascending order.
    void Sort()
    {
        // TODO: quick sort
    }

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
        allocProxy.deallocate(np, 1);
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
        size = 0;
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

    void InsertRange(iterator pos, iterator first, iterator last)
    {
        for (; first != last; ++first)
        {
            InsertNode(pos, *first);
        }
    }

    void IncreaseSize(size_t count)
    {
        size += count;
    }

    void DecreaseSize(size_t count)
    {
        size -= count;
    }

    // move elements from [first, last) at pos.
    void Transfer(iterator pos, iterator first, iterator last)
    {
        if (first != last)
        {
            // unlink [first, last) from old list
            NodePtr firstNode = first.nodePtr;
            NodePtr endNode = last.nodePtr->prev;

            NodePtr sourcePrevNode = firstNode->prev;
            NodePtr SourceNextNode = last.nodePtr;

            // unlink: reset old list
            sourcePrevNode->next = SourceNextNode;
            SourceNextNode->prev = sourcePrevNode;

            // unlink reset to-be-moved node in old list and link to new list
            NodePtr destPosNode = pos.nodePtr;
            NodePtr destPrevNode = destPosNode->prev;

            firstNode->prev = destPrevNode;
            endNode->next = destPosNode;

            // relink for new list
            destPrevNode->next = firstNode;
            destPosNode->prev = endNode;
        }
    }

private:
    std::allocator<T> alloc;
    NodePtr head;// head node of list, make list meets the stl [) range.
    size_t size;// number of elements.
};

// test routines for vector
template<typename T>
void PrintList(T t)
{
    // iterator test
    // use range to output elements
    for (auto v : t)
    {
        cout << v << " ";
    }
    cout << endl;
}

void TestSTDList()
{
    list<int> lst1;
    list<int> lst2(5, 1);
    list<int> lst3(5);
    list<int> lst4{ 1, 2, 3, 4, 5 };
    list<int> lst5(lst3.begin(), lst3.end());
    list<int> lst6(lst5);
    list<int> lst7(std::move(lst6));

    PrintList(lst1);
    PrintList(lst2);
    PrintList(lst3);
    PrintList(lst4);
    PrintList(lst5);
    PrintList(lst6);
    PrintList(lst7);

    // test clear
    lst7.clear();
    PrintList(lst7);

    // test insert n val
    lst4.insert(lst4.begin(), 2, 9);
    PrintList(lst4);

    lst4.insert(++lst4.begin(), 2, 8);
    PrintList(lst4);

    lst4.unique();
    PrintList(lst4);

    // remove
    lst4.remove(9);
    PrintList(lst4);

    // splice
    lst4.splice(lst4.begin(), lst2);
    PrintList(lst4);

    // reverse
    lst4.reverse();
    PrintList(lst4);

    // test erase range
    lst4.erase(lst4.begin(), lst4.end());
    PrintList(lst4);

    cout << "end of test std vector." << endl;
}

void TestMyList()
{
    List<int> lst1;
    List<int> lst2(5, 1);
    List<int> lst3(5);
    List<int> lst4;
    for (int i = 1; i < 6; i++)
    {
        lst4.Push_Back(i);
    }
    List<int> lst5(lst3.begin(), lst3.end());
    List<int> lst6(lst5);
    List<int> lst7(std::move(lst6));

    PrintList(lst1);
    PrintList(lst2);
    PrintList(lst3);
    PrintList(lst4);
    PrintList(lst5);
    PrintList(lst6);
    PrintList(lst7);

    // test clear
    lst7.Clear();
    PrintList(lst7);

    // test insert n val
    lst4.Insert(lst4.Begin(), 2, 9);
    PrintList(lst4);

    lst4.Insert(++lst4.Begin(), 2, 8);
    PrintList(lst4);

    lst4.Unique();
    PrintList(lst4);

    // remove
    lst4.Remove(9);
    PrintList(lst4);

    // splice
    lst4.Splice(lst4.Begin(), lst2);
    PrintList(lst4);

    // reverse
    lst4.Reverse();
    PrintList(lst4);

    // test erase range
    lst4.Erase(lst4.Begin(), lst4.End());
    PrintList(lst4);

    cout << "end of test List." << endl;
}

void TestList()
{
    TestSTDList();
    TestMyList();
}

#endif

