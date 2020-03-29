
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "UVLoop.h"

using namespace XSpace;

class MyLoop : public UVLoop
{
    public:
        MyLoop(const std::string& name) : UVLoop(name) {}
};

int main(int argc, char* argv[])
{
    #if 0
    UVLoop loop("Hello");
    loop.Start();
    #else
    std::shared_ptr<UVLoop> loop = UVLoop::Create<MyLoop>("Hello");
    loop->Start();
    #endif
	return 0;
}
