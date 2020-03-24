
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVLoop.h"
#include "NetSliceStream.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("");
    NetSliceStream nss(loop);

    nss.Bind("127.0.0.1", 13200);
    nss.Listen(10000);

    loop->Start();

    delete loop;
	return 0;
}
