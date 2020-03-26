
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

void test1()
{
    UVLoop *loop = new UVLoop("Loop");
    UVTcp *tcp = UVTcp::Create(loop);
    tcp->StartConnect("127.0.0.1", 13300);
    loop->Start();
    UVTcp::Destroy(tcp);
    delete loop;
}

void *pp = Allocator::malloc(64 * 1024);
bool writed = false;
int length = 0;
Msg msg1;

class MyTcp;
void test4(MyTcp *tcp);

class MyTcp : public UVTcp
{
public:
    MyTcp(UVLoop *loop) : UVTcp(loop)
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

    void Release()
    {
        DEBUG("\n");
        UVTcp::Release();
    }
};

// 一个包分多次发送
void test2()
{
    UVLoop *loop = new UVLoop("Loop");
    MyTcp *tcp = new MyTcp(loop);
    tcp->SetNoDelay();
    tcp->SetBlocking(true);
    tcp->StartConnect("127.0.0.1", 13200);
    tcp->StartRead();
    loop->Start();
    delete loop;
}

int main(int argc, char *argv[])
{
    test2();
    return 0;
}
