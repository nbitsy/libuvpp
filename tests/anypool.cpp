
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

#include "AnyPool.h"
#include "Debuger.h"

using namespace XNode;

struct Object
{
    Object() : a(100), b(200), c(1.3)
    {
        DEBUG("Object @%p\n", this);
    }
    Object(int a, int b, float c) : a(a), b(b), c(c)
    {
        DEBUG("Object @%p\n", this);
    }
    ~Object()
    {
        DEBUG("Object @%p\n", this);
    }

    int a;
    int b;
    float c;
};

int main(int argc, char *argv[])
{
    AnyPool pool;

    Object* obj = pool.Construct<Object>();
    Object* obj1 = pool.Construct<Object>(1,2,89.9);

    return 0;
}
