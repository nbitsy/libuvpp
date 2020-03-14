
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

#include "ObjectPool.h"
#include "uv.h"
#include <list>

using namespace XNode;

void test1()
{
    ObjectPool<uv_loop_t> pool(3);
    uv_loop_t *loop = pool.Construct();
    uv_loop_t *loop1 = pool.Construct();
    uv_loop_t *loop2 = pool.Construct();

    pool.Destroy(loop2);
    pool.Destroy(loop1);
    pool.Destroy(loop);
    uv_loop_t *loop3 = pool.Construct();
    pool.Destroy(loop3);
}

void test2()
{
    ObjectPool<uv_loop_t> pool(2);
    uv_loop_t *loop = pool.Construct(); 
    uv_loop_t *loop1 = pool.Construct();
    uv_loop_t *loop2 = pool.Construct();
    uv_loop_t *loop3 = pool.Construct();
    uv_loop_t *loop4 = pool.Construct();
    pool.Destroy(loop4);
    pool.Destroy(loop);
    pool.Destroy(loop1);
    uv_loop_t *loop5 = pool.Construct();
    pool.Destroy(loop2);
    pool.Destroy(loop3);
    pool.Destroy(loop5);
}

void test3()
{
    std::list<uv_loop_t *> mems;
    ObjectPool<uv_loop_t> *pool = new ObjectPool<uv_loop_t>(10);
    for (int i = 0; i < 10000; ++i)
    {
        mems.push_back(pool->Construct());
        int r = random() % 3;
        if (r)
        {
            for (int i = 0; i < r; ++i)
            {
                if (mems.size())
                {
                    pool->Destroy(mems.back());
                    mems.pop_back();
                }
            }
        }
    }

    pool->PrintInfo();
    delete pool;
}

class PoolObj
{
public:
    PoolObj()
    {
        DEBUG("\n");
    }
    PoolObj(const char* p)
    {
        DEBUG("%s\n", p);
    }

    void* p;
};

void test4()
{
    ObjectPool<PoolObj> _pool(10);
    PoolObj* p1 = _pool.Construct();
    PoolObj* p2 = _pool.Construct("YYF");
}

int main(int argc, char *argv[])
{
    srandom(time(NULL));
    test1();
    test2();
    test3();
    test4();
    return 0;
}
