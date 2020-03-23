
#ifndef _ZIPARRAY_H_
#define _ZIPARRAY_H_

namespace XSpace
{
/*
属性索引位大小  位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	使用位图占空间数	真实内存比	省出内存比	属性个数比
1				32		    1				    256			    1024			    36				    3.52        96.48       256
2				64		    2				    256	            1024	            72	                7.03        92.97       128
3				96		    8				    256	            1024	            128	                12.5	    87.5	    32
4				128		    16				    256	            1024	            192	                18.75	    81.25	    16
5				160		    32	                256	            1024	            288	                28.13	    71.88       8
6				192		    64	                256	            1024	            448	                43.75	    56.25	    4
7				224		    128         	    256	            1024	            736	                71.88	    28.13       2
8				256		    256         	    256	            1024	            1280	            125	        -25	        1
								
								
属性索引位大小	位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	使用位图占空间数	真实内存比	省出内存比	属性个数比
1	            32	        1	                128	            512	                36	                7.03        92.97       128
2	            64	        2	                128	            512	                72	                14.06       85.94       64
3	            96	        8	                128	            512	                128	                25	        75	        16
4	            128	        16	                128	            512	                192	                37.5	    62.5	    8
5	            160	        32	                128	            512	                288	                56.25	    43.75	    4
6	            192	        64	                128	            512	                448	                87.5	    12.5	    2
7	            224	        128	                128	            512	                736	                143.75	    -43.75	    1
								
								
属性索引位大小	位图大小	存储的属性最大个数	属性最大个数	不用位图占空间数	使用位图占空间数	真实内存比	省出内存比	属性个数比
1	            32	        1	                64	            256	                36	                14.06       85.94       64
2	            64	        2	                64	            256	                72	                28.13       71.88       32
3	            96	        8	                64	            256	                128	                50	        50	        8
4	            128	        16	                64	            256	                192	                75	        25	        4
5	            160	        32	                64	            256	                288	                112.5	    -12.5	    2
6	            192	        64	                64	            256	                448	                175	        -75	        1
								
								
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
    int &SetRetRef(int idx, int value);
    int Clear(int idx);

    inline bool HasData(int idx) const { return HasData(idx, 0, 0); }

    void Print();

private:
    bool HasData(int idx, int **ppvalue, void *map, void* data, int datatype, int idxwidth, int idxempty);
    bool HasData(int idx, int **pvalue, int *realidx) const;
    void Init(int idxmax, int part = 1);
    void InitFull(int idxmax);
    void InitHashMap(int idxmax);
    void CopyDataFromOld(void *src, void *olddata, int datatype, int idxmax, int idxwidth, int idxempty);
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
    // 空索引
    unsigned char _idxempty;
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
