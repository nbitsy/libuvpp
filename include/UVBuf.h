
#ifndef UVBUF_H_
#define UVBUF_H_

namespace XNode
{

class UVBuf
{
private:
    UVBuf(void* data, int length) : _data(data), _length(length) {}
    ~UVBuf() {}

    inline int Length() const { return _length; }
    inline void* Data() const { return _data; }

private:
    void* _data;
    int _length;
};

} // namespace XNode

#endif // UVBUF_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

