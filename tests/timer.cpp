
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVLoop.h"
#include "UVTimer.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    std::shared_ptr<UVLoop> loop = UVLoop::Create<>("");
    std::weak_ptr<UVLoop> wl(loop);
    std::shared_ptr<UVTimer> timer4close = UVTimer::Create<>(wl);
    std::shared_ptr<UVTimer> timer = UVTimer::Create<>(wl);
    timer->Start(1000, 3, 3);
    loop->Start();
	return 0;
}
