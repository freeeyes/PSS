#ifndef _ISERVERMANAGER_H
#define _ISERVERMANAGER_H

#include <stdio.h>

class IServerManager
{
public:
    virtual ~IServerManager() {}
    virtual bool Close() = 0;
};

#endif
