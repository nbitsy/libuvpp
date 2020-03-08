
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVTimer.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Test");

    UVTimer* timer4close = UVTimer::Create(loop);
    UVTimer::Destroy(timer4close);

    UVTimer* timer = UVTimer::Create(loop);
    timer->Start(1000, 3);
    loop->Run();

    UVTimer::Destroy(timer);
    delete loop;
	return 0;
}
