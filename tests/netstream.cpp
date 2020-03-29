
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVLoop.h"
#include "NetSliceStream.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    std::shared_ptr<UVLoop> loop = UVLoop::Create<>("Loop");
    std::weak_ptr<UVLoop> l(loop);
    std::shared_ptr<NetSliceStream> nss = NetSliceStream::Create<>(l);

    nss->Bind("127.0.0.1", 13200);
    nss->Listen(10000);
    loop->Start();

	return 0;
}
