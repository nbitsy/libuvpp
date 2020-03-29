
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
    std::shared_ptr<UVTcp> tcp4close = UVTcp::Create<>(l);
    std::shared_ptr<UVTcp> tcp = UVTcp::Create<>(l);

    tcp->Bind("127.0.0.1", 13200);
    tcp->Listen(10000);
    loop->Start();
	return 0;
}
