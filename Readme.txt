
本系统依赖
    libuv >= 1.34.3
    tcmalloc >= 2.7

TODO:
内存分配器使用tcmalloc
对象池

重要提示：
    1,必须确保UVHandle对象是在OnClosed之后释放