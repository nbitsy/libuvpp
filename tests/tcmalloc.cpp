
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "TCAllocator.h"
#include <iostream>

using namespace XSpace;

int main(int argc, char* argv[])
{
    void* p = TCAllocator::malloc(100);
    std::cout << "Malloc @" << p << std::endl;
    TCAllocator::free(p);
    std::cout << "Free @" << p << std::endl;

	return 0;
}
