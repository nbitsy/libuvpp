
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVPP.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Loop");

    UVUdp* udp4close = UVUdp::Create(loop);
    UVUdp::Destroy(udp4close);

    UVUdp* udp = UVUdp::Create(loop);

    udp->Bind("127.0.0.1", 13300, UV_UDP_REUSEADDR);
    // udp->SetBroadcast(true);
    udp->StartRead();

    loop->Start();

    UVUdp::Destroy(udp);
    delete loop;
	return 0;
}
