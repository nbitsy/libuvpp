
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
    std::shared_ptr<UVLoop> loop = UVLoop::Create<>("TEst");
    std::weak_ptr<UVLoop> l(loop);
    std::shared_ptr<UVIdle> obj = UVIdle::Create(l);
    obj->Start();
    loop->Start();
    obj->Stop();
	return 0;
}
