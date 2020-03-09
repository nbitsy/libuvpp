
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVTcp.h"
#include "UVLoop.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Test");

    UVTcp* tcp4close = UVTcp::Create(loop);
    UVTcp::Destroy(tcp4close);

    UVTcp* tcp = UVTcp::Create(loop);

    tcp->Bind("127.0.0.1", 13300);
    tcp->Listen(10000);
    loop->Start();

    UVTcp::Destroy(tcp);
    delete loop;
	return 0;
}
