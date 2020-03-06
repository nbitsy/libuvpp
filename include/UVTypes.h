
#ifndef UVTYPES_H_
#define UVTYPES_H_

#include <string>

namespace XNode
{

struct EndPointAddress
{
    EndPointAddress() : _port(0) {}
    EndPointAddress(const std::string& ip, int port) : _ip(ip), _port(port) {}

    std::string _ip;
    int _port;
};

} // namespace XNode

#endif // UVTYPES_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

