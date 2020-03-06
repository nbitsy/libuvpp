
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVLoop.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    UVLoop loop("Hello");
    loop.Run();
	return 0;
}
