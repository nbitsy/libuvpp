
#include "UVCheck.h"

namespace XNode
{

static void __OnCheck(uv_check_t *handle)
{
    UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)handle);
    if (NULL == uvdata)
        return;

    if (uvdata->_self != NULL)
        ((UVCheck *)uvdata->_self)->OnCheck();
}

UVCheck::UVCheck(UVLoop *loop) : UVHandle(loop)
{
    _handle = (uv_handle_t *)malloc(sizeof(uv_check_t));
    if (_loop != NULL && _handle != NULL)
        uv_check_init(loop->GetLoop<uv_loop_t>(), (uv_check_t *)_handle);

    SetData(NULL);
    std::cout << "Object@" << (void *)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

UVCheck::~UVCheck()
{
    std::cout << "Object@" << (void *)this << " =>" << __PRETTY_FUNCTION__ << std::endl;
}

bool UVCheck::Start()
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_check_start((uv_check_t *)_handle, __OnCheck);
}

bool UVCheck::Stop()
{
    if (NULL == _loop || NULL == _handle)
        return false;

    return !uv_check_stop((uv_check_t *)_handle);
}

void UVCheck::OnClosed()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void UVCheck::OnCheck()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
