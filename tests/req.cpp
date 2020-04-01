
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVLoop.h"
#include "UVReqWrite.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    std::shared_ptr<UVLoop> loop = UVLoop::Create<>("");
    std::weak_ptr<UVLoop> wl(loop);

	return 0;
}
