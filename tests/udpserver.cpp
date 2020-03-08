
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVUdp.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Loop");

    UVUdp* udp4close = UVUdp::Create(loop);
    udp4close->Close();
    // UVUdp::Destroy(udp4close);

    UVUdp* udp = UVUdp::Create(loop);

    udp->Bind("127.0.0.1", 13301);
    udp->BeginRead();

    loop->Run();

    UVUdp::Destroy(udp);
    delete loop;
	return 0;
}
