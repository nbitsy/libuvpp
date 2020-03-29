
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
    std::shared_ptr<UVTty> tty = UVTty::Create<>(l, 1, true);
    const char* message = "***********************************FUCK\n";
    tty->StartRead();
    tty->Write((void*)message, strlen(message));

    std::shared_ptr<UVTimer> timer = UVTimer::Create<>(l);
    timer->Start(1000, 3);

    loop->Start();
	return 0;
}
