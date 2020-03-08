
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVPrepare.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Test");

    UVPrepare* obj = new UVPrepare(loop);
    obj->Start();
    loop->Run();

    obj->Stop();
    delete loop;
	return 0;
}
