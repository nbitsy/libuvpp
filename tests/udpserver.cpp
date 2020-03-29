
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVPP.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    std::shared_ptr<UVLoop> loop = UVLoop::Create<>("Loop");
    std::weak_ptr<UVLoop> l(loop);
    std::shared_ptr<UVUdp> udp4close = UVUdp::Create<>(l);
    std::shared_ptr<UVUdp> udp = UVUdp::Create<>(l);

    udp->Bind("127.0.0.1", 13300, UV_UDP_REUSEADDR);
    // udp->SetBroadcast(true);
    udp->StartRead();
    loop->Start();
	return 0;
}
