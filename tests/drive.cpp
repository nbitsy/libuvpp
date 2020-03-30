
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "TypeTraits.h"
#include "Debugger.h"

using namespace XSpace;

class A {};
class B : public A {};

int main(int argc, char* argv[])
{
    DEBUG("%d\n", is_drived_from<B, A>::value);
	return 0;
}
