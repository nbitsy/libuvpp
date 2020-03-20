
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVPP.h"
//#include "UVIdle.h"
//#include "UVLoop.h"

using namespace XSpace;

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
