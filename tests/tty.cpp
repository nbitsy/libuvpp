
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVPP.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop* loop = new UVLoop("Test");
    UVTty* tty = new UVTty(loop, 1, true);
    const char* message = "***********************************FUCK\n";
    tty->StartRead();
    tty->Write((void*)message, strlen(message));

    UVTimer* timer = UVTimer::Create(loop);
    timer->Start(1000, 3);

    loop->Start();
    delete loop;
	return 0;
}
