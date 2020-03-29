
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

#include "Allocator.h"
#include "Slice.h"
#include "Msg.h"
#include "UVPP.h"

using namespace XSpace;

void *pp = Allocator::malloc(64 * 1024);
bool writed = false;
int length = 0;
Msg msg1;

class MyTcp;
void test4(MyTcp *tcp);

class MyTcp : public UVTcp
{
public:
    MyTcp(std::weak_ptr<UVLoop>& loop) : UVTcp(loop)
    {
        DEBUG("\n");
    }
    ~MyTcp()
    {
        DEBUG("\n");
    }

    void OnConnected()
    {
        Slice *s = new (pp) Slice();
        s->Length = sizeof(*s) + sizeof(msg1) + 6;
        length = s->Length;
        msg1.MsgLength = sizeof(msg1) + 6;
        memcpy(msg1.Body(), "fuck u", 6);
        memcpy(s->Body(), &msg1, sizeof(msg1)+6);
        this->Write(pp, 3);
    }

    void OnRead(void *data, int nread)
    {
        if (!writed)
        {
            this->Write((char *)pp + 3, length - 3);
            writed = true;
        }
    }
};

// 一个包分多次发送
void test2()
{
    std::shared_ptr<UVLoop> loop = UVLoop::Create<>("Loop");
    std::weak_ptr<UVLoop> l(loop);
    std::shared_ptr<UVTcp> tcp = MyTcp::Create<MyTcp>(l);

    tcp->SetNoDelay();
    tcp->SetBlocking(true);
    tcp->StartConnect("127.0.0.1", 13200);
    tcp->StartRead();
    loop->Start();
}

int main(int argc, char *argv[])
{
    test2();
    return 0;
}
