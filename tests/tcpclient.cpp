
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVPP.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Loop");
    UVTcp* tcp = UVTcp::Create(loop);
    tcp->StartConnect("127.0.0.1", 13300);
    loop->Start();
    UVTcp::Destroy(tcp);
    delete loop;
	return 0;
}
