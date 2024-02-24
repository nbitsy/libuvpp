
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include <iostream>
#include "UVPP.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    std::shared_ptr<UVLoop> loop = UVLoop::Create<>("Loop");
    std::shared_ptr<UVTcp> tcp4close = UVTcp::Create<>(loop->weak_from_this());

    std::weak_ptr<UVLoop> l(loop);
    std::shared_ptr<UVTcp> tcp = UVTcp::Create<>(l);
    tcp->Bind("127.0.0.1", 13200);
    tcp->Listen(10000);
    loop->Start();
	return 0;
}
