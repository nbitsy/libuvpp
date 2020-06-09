
#ifndef _MINHEAP_H_
#define _MINHEAP_H_

#include "Allocator.h"
#include <unordered_map>

namespace XSpace
{

template <typename T, typename Greater>
struct MinHeapNode
{
    MinHeapNode(const T& value = T())
        : Value(value), Idx(-1), Next(0), Prev(0) {}
    ~MinHeapNode() {}

    inline bool operator>(const MinHeapNode& n) const
    {
        Greater gt;
        return gt(Value, n.Value);
    }

    T Value;
    int Idx;
    struct MinHeapNode* Next;
    struct MinHeapNode** Prev;
};

template <typename T, typename Greater>
class MinHeap
{
public:
    typedef size_t size_type;
    typedef MinHeapNode<T, Greater> value_type;
    typedef value_type* node_pointer;

public:
    MinHeap() : _data(0), _size(0), _capacity(0) {}
    ~MinHeap();

    inline size_type Size() const { return _size; }
    inline bool Empty() const { return 0 == _size; }
    inline bool IsTop(const node_pointer v) const { return v ? v->Idx == 0 : false; }
    inline node_pointer Top() const { return _size ? *_data : 0; }
    inline int Reserve(size_type n);

    node_pointer Push(const T& v);
    node_pointer Push(node_pointer v);
    node_pointer Pop();
    int Erase(const node_pointer v);
    int Erase(const T& v);

protected:
    void ShiftUp(unsigned hole_index, const node_pointer v);
    void ShiftDown(unsigned hole_index, const node_pointer v);

private:
    // TODO: 把节点缓存起来
    inline void* realloc(void* ptr, size_type size) { return Allocator::realloc(ptr, size); }
    inline void* malloc(size_type size) { return Allocator::malloc(size); }
    inline void free(void* ptr) { Allocator::free(ptr); }

private:
    // 元素存储空间
    node_pointer* _data;
    // 当前元素个数
    mutable size_type _size;
    // 容量大小
    size_type _capacity;
    // 值到节点的映射，方便基于值的删除操作
    std::unordered_map<T, node_pointer> _v2p;
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
typename MinHeap<T, Greater>::node_pointer MinHeap<T, Greater>::Push(const T& val)
{
    if (Reserve(_size + 16))
        return 0;

    node_pointer v = Allocator::Construct<value_type>(val);
    if (!v)
        return 0;

    _v2p.insert(std::make_pair(val, v));
    ShiftUp(_size++, v);
    return v;
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::node_pointer MinHeap<T, Greater>::Push(node_pointer v)
{
    if (Reserve(_size + 16))
        return 0;

    ShiftUp(_size++, v);
    return v;
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::node_pointer MinHeap<T, Greater>::Pop()
{
    if (_size)
    {
        node_pointer v = *_data;
        ShiftDown(0, _data[--_size]);
        v->Idx = -1;
        return v;
    }
    return 0;
}

template <typename T, typename Greater>
int MinHeap<T, Greater>::Erase(const T& v)
{
    auto i = _v2p.find(v);
    if (i == _v2p.end())
        return -1;

    auto& n = i->second;
    if (n)
        return Erase(n);
    return -1;
}

template <typename T, typename Greater>
int MinHeap<T, Greater>::Erase(const node_pointer v)
{
    if (!v)
        return -1;

    if (v->Idx != -1)
    {
        _v2p.erase(v->Value);

        node_pointer last = _data[--_size];
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

        node_pointer* ptr = 0;
        if (!(ptr = (node_pointer*)realloc(_data, capacity * sizeof(node_pointer))))
            return -1;

        _data = ptr;
        _capacity = capacity;
    }

    return 0;
}

template <typename T, typename Greater>
void MinHeap<T, Greater>::ShiftUp(unsigned hole_index, const node_pointer v)
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
void MinHeap<T, Greater>::ShiftDown(unsigned hole_index, const node_pointer v)
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
