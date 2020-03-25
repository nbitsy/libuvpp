
#ifndef _NETADDRESS_H_
#define _NETADDRESS_H_

#include <string>

namespace XSpace
{

struct NetAddress
{
    NetAddress() : Port(0) {}
    NetAddress(const std::string &ip, int port) : Ip(ip), Port(port) {}
    NetAddress(const NetAddress &other) : Ip(other.Ip), Port(other.Port) {}

    NetAddress &operator=(NetAddress &other)
    {
        Ip = other.Ip;
        Port = other.Port;
        return *this;
    }

    inline std::string ToString() const { return Ip + ":" + std::to_string(Port); }

    std::string Ip;
    int Port;
};

} // namespace XSpace

#endif // _NETADDRESS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
