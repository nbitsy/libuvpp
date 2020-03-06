
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVTcp.h"
#include "UVLoop.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop loop("Test");
    UVTcp tcp(&loop);

    loop.SetGC(false);
    tcp.SetGC(false);

    tcp.Bind("127.0.0.1", 13300);
    tcp.Listen(10000);
    loop.Run();

	return 0;
}
