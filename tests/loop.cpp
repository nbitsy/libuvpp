
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVLoop.h"

using namespace XSpace;

int main(int argc, char* argv[])
{
    UVLoop loop("Hello");
    loop.Start();
	return 0;
}
