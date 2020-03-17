
#ifndef _ENDPOINTADDRESS_H_
#define _ENDPOINTADDRESS_H_

#include <string>

namespace XNode
{

struct EndPointAddress
{
    EndPointAddress() : _port(0) {}
    EndPointAddress(const std::string& ip, int port) : _ip(ip), _port(port) {}
    EndPointAddress(const EndPointAddress& other) : _ip(other._ip), _port(other._port) {}

    inline std::string ToString() const { return _ip + ":" + std::to_string(_port); }

    std::string _ip;
    int _port;
};

} // namespace XNode

#endif // _ENDPOINTADDRESS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
