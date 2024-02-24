
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "MemPool.h"
#include "Allocator.h"
#include <list>
#include <math.h>

using namespace XNode;

void test1()
{
    MemPool<> pool;
    void* p1 = pool.AllocBlock(1);
    void* p2 = pool.AllocBlock(64);
    pool.FreeBlock(p1);
    pool.PrintInfo();
    void* p3 = pool.AllocBlock(65);
    pool.FreeBlock(p2);
    pool.FreeBlock(p3);
    pool.PrintInfo();
    void* p4 = pool.AllocBlock(64);
    void* p5 = pool.AllocBlock(32);
    pool.FreeBlock(p4);
    pool.FreeBlock(p5);
    pool.PrintInfo();
    //pool.FreeBlock(p5);
}

void test2()
{
    std::list<void*> mems;
    MemPool<> pool;

    for (int i = 0; i < 10000; ++i)
    {
        int size = random()%(MEMPOOL_SIZE_MAX*2);
        void* p = pool.AllocBlock(size);
        mems.push_back(p);

        int f = random()%2;
        if (f && mems.size() > 0)
        {
            void* p = mems.back();
            mems.pop_back();
            pool.FreeBlock(p);
        }
    }

    pool.PrintInfo();

    for (int i = 0; i < mems.size(); ++i)
    {
        void* p = mems.front();
        pool.FreeBlock(p);
        mems.pop_front();
    }

    pool.PrintInfo();
}

int main(int argc, char* argv[])
{
    test1();
    test2();

    MemPool<Allocator> pool;
	return 0;
}
