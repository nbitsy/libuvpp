
#ifndef _MINHEAP_H_
#define _MINHEAP_H_

#include "Allocator.h"

namespace XSpace
{

template <typename T, typename Greater>
struct MinHeapNode
{
    MinHeapNode(const T &value = T())
        : Value(value), Idx(-1), Next(0), Prev(0) {}
    ~MinHeapNode() {}

    inline bool operator>(const MinHeapNode &n) const
    {
        Greater gt;
        return gt(Value, n.Value);
    }

    T Value;
    int Idx;
    struct MinHeapNode *Next;
    struct MinHeapNode **Prev;
};

template <typename T, typename Greater>
class MinHeap
{
public:
    typedef size_t size_type;
    typedef MinHeapNode<T, Greater> value_type;
    typedef value_type *pointer;

public:
    MinHeap() : _data(0), _size(0), _capacity(0) {}
    ~MinHeap();

    inline size_type Size() const { return _size; }
    inline bool Empty() const { return 0 == _size; }
    inline bool IsTop(const pointer v) const { return v ? v->Idx == 0 : false; }
    inline pointer Top() const { return _size ? *_data : 0; }

    int Reserve(size_type n);
    pointer Push(const T &v);
    pointer Push(pointer v);
    pointer Pop();
    int Erase(const pointer v);

protected:
    void ShiftUp(unsigned hole_index, const pointer v);
    void ShiftDown(unsigned hole_index, const pointer v);

private:
    inline void *realloc(void *ptr, size_type size) { return Allocator::realloc(ptr, size); }
    inline void *malloc(size_type size) { return Allocator::malloc(size); }
    inline void free(void *ptr) { Allocator::free(ptr); }

private:
    pointer *_data;
    mutable size_type _size;
    size_type _capacity;
};

template <typename T, typename Greater>
MinHeap<T, Greater>::~MinHeap()
{
    for (unsigned i = 0; i < _size; ++i)
    {
        if (_data[i])
            free(_data[i]);
    }
    free(_data);
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::pointer MinHeap<T, Greater>::Push(const T &val)
{
    if (Reserve(_size + 16))
        return 0;

    pointer v = Allocator::Construct<value_type>(val);
    if (!v)
        return 0;

    ShiftUp(_size++, v);
    return v;
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::pointer MinHeap<T, Greater>::Push(pointer v)
{
    if (Reserve(_size + 16))
        return 0;

    ShiftUp(_size++, v);
    return v;
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::pointer MinHeap<T, Greater>::Pop()
{
    if (_size)
    {
        pointer v = *_data;
        ShiftDown(0, _data[--_size]);
        v->Idx = -1;
        return v;
    }
    return 0;
}

template <typename T, typename Greater>
int MinHeap<T, Greater>::Erase(const pointer v)
{
    if (!v)
        return -1;

    if (v->Idx != -1)
    {
        pointer last = _data[--_size];
        unsigned parent = (v->Idx - 1) / 2;
        if (v->Idx > 0 && *_data[parent] > *last)
            ShiftUp(v->Idx, last);
        else
            ShiftDown(v->Idx, last);
        v->Idx = -1;

        free(v);
        return 0;
    }
    return -1;
}

template <typename T, typename Greater>
int MinHeap<T, Greater>::Reserve(size_type n)
{
    if (_capacity < n)
    {
        size_type capacity = _capacity ? _capacity * 2 : 8;
        if (capacity < n)
            capacity = n;

        pointer *ptr = 0;
        if (!(ptr = (pointer *)realloc(_data, capacity * sizeof(pointer))))
            return -1;

        _data = ptr;
        _capacity = capacity;
    }

    return 0;
}

template <typename T, typename Greater>
void MinHeap<T, Greater>::ShiftUp(unsigned hole_index, const pointer v)
{
    unsigned parent = (hole_index - 1) / 2;
    while (hole_index && (*_data[parent] > *v)) // XXX: >
    {
        (_data[hole_index] = _data[parent])->Idx = hole_index;
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    }
    (_data[hole_index] = v)->Idx = hole_index;
}

template <typename T, typename Greater>
void MinHeap<T, Greater>::ShiftDown(unsigned hole_index, const pointer v)
{
    unsigned min_child = 2 * (hole_index + 1);
    while (min_child <= _size)
    {
        min_child -= min_child == _size || (*_data[min_child] > *_data[min_child - 1]);
        if (!(*v > *_data[min_child]))
            break;
        (_data[hole_index] = _data[min_child])->Idx = hole_index;
        hole_index = min_child;
        min_child = 2 * (hole_index + 1);
    }
    (_data[hole_index] = v)->Idx = hole_index;
}

} // namespace XSpace

#endif // _MINHEAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
