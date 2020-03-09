
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "MemPool.h"

using namespace XNode;

int main(int argc, char* argv[])
{
    MemPool<> pool;

    void* p = pool.AllocBlock(1);
    pool.FreeBlock(p);

	return 0;
}
