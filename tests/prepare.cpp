
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVPP.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    std::shared_ptr<UVLoop> loop = UVLoop::Create<>("Loop");
    std::weak_ptr<UVLoop> l(loop);
    std::shared_ptr<UVPrepare> obj = UVPrepare::Create<UVPrepare>(l);

    obj->Start();
    loop->Start();
    obj->Stop();

	return 0;
}
