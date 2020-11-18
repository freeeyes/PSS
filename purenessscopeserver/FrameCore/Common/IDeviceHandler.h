#ifndef _DEVICE_HANDLER_H
#define _DEVICE_HANDLER_H

class IDeviceHandler
{
public:
    virtual ~IDeviceHandler() = default;

    virtual bool Device_Send_Data(const char* pData, ssize_t nLen) = 0;
};

#endif
