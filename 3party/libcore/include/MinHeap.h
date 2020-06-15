
#ifndef _MINHEAP_H_
#define _MINHEAP_H_

#include <unordered_map>

#include "Allocator.h"
#include "TypeTraits.h"

namespace XSpace
{

#ifdef _DEBUG
#define RESERVED_SIZE 4
#define RESERVE_THRESHOLD 2
#define MINHEAP_SIZE_MIN 4
#else
#define RESERVED_SIZE 16
#define RESERVE_THRESHOLD 4
#define MINHEAP_SIZE_MIN 8
#endif

template <typename T>
struct greater
{
    typedef typename remove_const<T>::type value_type;
    inline bool operator()(const value_type& a, const value_type& b) const
    {
        return a > b;
    }
};

template <typename T, typename Greater = greater<T>>
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

template <typename T, typename Greater = greater<T>>
class MinHeap
{
public:
    typedef size_t size_type;
    typedef MinHeapNode<T, Greater> value_type;
    typedef value_type* node_pointer;

public:
    MinHeap() : _data(0), _size(0), _capacity(0) {}
    ~MinHeap();

    void Clear();
    inline size_type Size() const { return _size; }
    inline bool Empty() const { return 0 == _size; }
    inline bool IsTop(const node_pointer v) const { return v ? v->Idx == 0 : false; }
    inline node_pointer Top() const { return _size ? *_data : 0; }
    inline bool Reserve();

    inline size_type AvailableSize() const
    {
        if (_capacity <= _size)
            return 0;

        return _capacity - _size;
    }

    node_pointer Push(const T& v);
    node_pointer Push(node_pointer v);
    node_pointer Pop();
    int Erase(const node_pointer v);
    int Erase(const T& v);

    node_pointer Adjust(const node_pointer v);
    node_pointer Adjust(const T& v);

protected:
    void ShiftUp(unsigned hole_index, const node_pointer v);
    void ShiftUpunconditional(unsigned hole_index, const node_pointer v);
    void ShiftDown(unsigned hole_index, const node_pointer v);

private:
    // TODO: 把节点缓存起来
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
void MinHeap<T, Greater>::Clear()
{
    _v2p.clear();

    for (unsigned i = 0; i < _size; ++i)
    {
        if (_data[i])
            free(_data[i]);
    }
    free(_data);

    _data = NULL;
    _capacity = 0;
    _size = 0;
}

template <typename T, typename Greater>
MinHeap<T, Greater>::~MinHeap()
{
    if (_data)
    {
        for (unsigned i = 0; i < _size; ++i)
        {
            if (_data[i])
                free(_data[i]);
        }
        free(_data);
    }
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::node_pointer MinHeap<T, Greater>::Push(const T& val)
{
    if (!Reserve())
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
    if (!Reserve())
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
    if (!v || !_size)
        return -1;

    if (v->Idx != -1)
    {
        _v2p.erase(v->Value);

        node_pointer last = _data[--_size];
        unsigned parent = (v->Idx - 1) / 2;
        if (v->Idx > 0 && *_data[parent] > *last)
            ShiftUpunconditional(v->Idx, last);
        else
            ShiftDown(v->Idx, last);

        v->Idx = -1;
        Allocator::Destroy(v);
        return 0;
    }

    return -1;
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::node_pointer MinHeap<T, Greater>::Adjust(const node_pointer v)
{
	if (-1== v->Idx) {
		return Push(v);
	} else {
		size_t parent = (v->Idx- 1) / 2;
		/* The position of e has changed; we shift it up or down
		 * as needed.  We can't need to do both. */
		if (v->Idx > 0 && *_data[parent] > *v)
			ShiftUpunconditional(v->Idx, v);
		else
            ShiftDown(v->Idx, v);

		return v;
	}

    return NULL;
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::node_pointer MinHeap<T, Greater>::Adjust(const T& v)
{
    auto i = _v2p.find(v);
    if (i == _v2p.end())
        return NULL;

    auto& n = i->second;
    if (n)
        return Adjust(n);

    return NULL;
}

template <typename T, typename Greater>
bool MinHeap<T, Greater>::Reserve()
{
    if (AvailableSize() > RESERVE_THRESHOLD)
        return true;

    size_type capacity = _capacity ? _capacity * 2 : MINHEAP_SIZE_MIN;
    if (capacity < RESERVED_SIZE)
        capacity = RESERVED_SIZE;

    int size = capacity * sizeof(node_pointer);
    node_pointer* ptr = (node_pointer*)malloc(size);
    if (!ptr)
        return false;

    std::memset(ptr, 0x00, size);
    if (_data)
    {
        std::memcpy(ptr, _data, _capacity * sizeof(node_pointer));
        free(_data);
    }

    _data = ptr;
    _capacity = capacity;

    return true;
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
void MinHeap<T, Greater>::ShiftUpunconditional(unsigned hole_index, const node_pointer v)
{
    size_t parent = (hole_index - 1) / 2;
    do
    {
        (_data[hole_index] = _data[parent])->Idx = hole_index;
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    } while (hole_index && *_data[parent] > *v);
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
