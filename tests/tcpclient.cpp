
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

#include "Allocator.h"
#include "Slice.h"
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
#if 0
        void *p = Allocator::malloc(64 * 1024);
        Slice *s = new (p) Slice();
        s->Length = 7;
        snprintf((char *)s->Body(), 4, "fuck");
        this->Write(p, 7);

        Slice* ss = new ((char*)p+7) Slice();
        ss->Length = 10;
        snprintf((char *)ss->Body(), 7, "fuck u");
        this->Write((char*)p+7, 10);
#endif
// 比包头还小的包
#if 1
        Slice *s = new (pp) Slice();
        s->Length = 10;
        snprintf((char *)s->Body(), 7, "fuck u");
        this->Write(pp, 3);
#endif
    }

    void OnRead(void *data, int nread)
    {
        if (!writed)
        {
            this->Write((char *)pp + 3, 7);
            writed = true;
            test4(this);
        }
    }

    void Release()
    {
        DEBUG("\n");
        UVTcp::Release();
    }
};

void test4(MyTcp *tcp)
{
    void *p = Allocator::malloc(64 * 1024);
    Slice *s = new (p) Slice();
    s->Length = 7;
    snprintf((char *)s->Body(), 4, "fuck");
    tcp->Write(p, 7);

    Slice *ss = new ((char *)p + 7) Slice();
    ss->Length = 10;
    snprintf((char *)ss->Body(), 7, "Im yyf");
    tcp->Write((char *)p + 7, 10);
}

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
    // delete tcp;
    delete loop;
}

int main(int argc, char *argv[])
{
    //test1();
    test2();
    return 0;
}
