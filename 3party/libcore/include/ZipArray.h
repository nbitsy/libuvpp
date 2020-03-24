
#ifndef _ZIPARRAY_H_
#define _ZIPARRAY_H_

namespace XSpace
{
/*
属性索引位大小	位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	管理数据占用空间	使用位图占空间数	真实内存比	省出内存比	属性个数比
1	32	1	256	1024	36	72	7.03125	92.96875	256
2	64	2	256	1024	36	108	10.546875	89.453125	128
3	96	8	256	1024	36	164	16.015625	83.984375	32
4	128	16	256	1024	36	228	22.265625	77.734375	16
5	160	32	256	1024	36	324	31.640625	68.359375	8
6	192	64	256	1024	36	484	47.265625	52.734375	4
7	224	128	256	1024	36	772	75.390625	24.609375	2
8	256	256	256	1024	36	1316	128.515625	-28.515625	1
									
									
属性索引位大小	位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	管理数据占用空间	使用位图占空间数	真实内存比	省出内存比	属性个数比
1	16	1	128	512	36	56	10.9375	89.0625	128
2	32	2	128	512	36	76	14.84375	85.15625	64
3	48	8	128	512	36	116	22.65625	77.34375	16
4	64	16	128	512	36	164	32.03125	67.96875	8
5	80	32	128	512	36	244	47.65625	52.34375	4
6	96	64	128	512	36	388	75.78125	24.21875	2
7	112	128	128	512	36	660	128.90625	-28.90625	1
									
									
属性索引位大小	位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	管理数据占用空间	使用位图占空间数	真实内存比	省出内存比	属性个数比
1	8	1	64	256	36	48	18.75	81.25	64
2	16	2	64	256	36	60	23.4375	76.5625	32
3	24	8	64	256	36	92	35.9375	64.0625	8
4	32	16	64	256	36	132	51.5625	48.4375	4
5	40	32	64	256	36	204	79.6875	20.3125	2
6	48	64	64	256	36	340	132.8125	-32.8125	1

结论：当出现的属性条数是最大属性条数的1/8时，可以省一半以上内存，而且会随着最大属性条数越多，省出的内存越多。								
当属性条数越过1/8时，可以转换成全属性存储。 1/4时的收受也是非常可观的。
XXX: 如果存储的数据是8字节收受会更可观。


如果存储的数据是4字节宽的，当数据条数超过1/8时使用全索引空间存储，也就是_value里会分配 _idxmax * sizeof(int) 个字节来存放数据。
否则只申请1/8的存储空间
如果存储的数据是8字节宽的，则申请1/4的 _idxmax * sizeof(long) 的空间来存储数据。当超过1/4时转为全空间存储。
*/

// 至少存储64个值
#define ZIP_ARRAY_SIZE_MIN 64 // XXX: 实际上只能存储63个值，因为第64个索引作为空索引
// 最多256个值，如果超过这个值则转换成hash_map/unordered_map
#define ZIP_ARRAY_SIZE_MAX 512

// 超过1/8时先切到1/4，超过1/4时切到全量
class ZipArray
{
public:
    const static int null;

public:
    explicit ZipArray(int idxmax = ZIP_ARRAY_SIZE_MIN, int type = 1);
    ~ZipArray();

    // 返回引用是一个危险的动作，所以需要明确的调用
    int &GetRef(int idx);
    inline int Get(int idx) { return GetRef(idx); }
    inline int operator[](int idx) { return GetRef(idx); }

    void Set(int idx, int value);
    // 清空并返回当前值
    int Clear(int idx);

    inline bool HasData(int idx) const { return HasData(idx, 0, 0); }

    void Print();

private:
    bool HasData(int idx, int **ppvalue, void *map, void* data, int datatype, int idxwidth);
    bool HasData(int idx, int **pvalue, int *realidx) const;
    void Init(int idxmax, int part = 1);
    void InitFull(int idxmax);
    void InitHashMap(int idxmax);
    void CopyDataFromOld(void *src, void *olddata, int datatype, int idxmax, int idxwidth);
    void ConvertTo_1_4();
    void ConvertToFull();
    void ConvertToHashMap();

private:
    // 当前元素个数
    int _count; // _count / _idxmax > 1/8 or _count / _idxmax > 1/4 切到全量
    // 当前空间大小
    int _size;
    // 索引最大值
    int _idxmax;
    // 索引位数
    int _idxwidth;
    // 当前数据空间类型
    unsigned char _datatype; // 0 - 1/8 1 - 1/4 2 - full 3 - hashmap
    // 数据，数据存放空间包括位图数据，位图数据之后是正常数据
    void *_value;
    // 真实数据起始地址
    void *_data;
};

} // namespace XSpace

#endif // _ZIPARRAY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
