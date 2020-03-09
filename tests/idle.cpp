
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVIdle.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Test");

    UVIdle* obj = new UVIdle(loop);
    obj->Start();
    loop->Start();

    obj->Stop();
    delete loop;
	return 0;
}
