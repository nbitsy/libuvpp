
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVTcp.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Loop");
    UVTcp* tcp = UVTcp::Create(loop);
    tcp->BeginConnect("127.0.0.1", 13300);
    loop->Run();
    UVTcp::Destroy(tcp);
    delete loop;
	return 0;
}
