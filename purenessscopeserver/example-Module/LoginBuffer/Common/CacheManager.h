#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H

//抽象Cache层的管理
//抽象公共层
//add by freeeyes

#include "SMOption.h"

class CCacheManager
{
public:
    CCacheManager() {};

    ~CCacheManager() {};

    //***********************************************
    //进程第一次打开缓冲
    //u4CachedCount : 缓冲块的个数
    //objMemorykey  ：共享内存key
    //u4CheckSize   ：单块缓冲大小
    //***********************************************
    bool Init(uint32 u4CachedCount, key_t objMemorykey, uint32 u4CheckSize)
    {
        bool blIsCreate = true;
        bool blIsOpen   = true;

        Set_Cache_Count(u4CachedCount);

        blIsOpen = m_SMOption.Init(objMemorykey, u4CheckSize, (uint16)Get_Cache_Count(), blIsCreate);

        if(false == blIsOpen)
        {
            return false;
        }

        if(blIsCreate == true)
        {
            //共享内存第一次创建，需要从文件里面重建共享内存
            Read_All_Init_DataResoure();
        }
        else
        {
            //共享内存已经存在，遍历获得列表
            Read_All_From_CacheMemory();
        }

        return true;
    };


    //***********************************************
    //关闭缓冲管理器，需要继承子类去实现之
    //***********************************************
    virtual void Close() {};

    //***********************************************
    //定时同步数据源和缓冲中的数据，需要继承子类去实现之(数据源 -> 内存)
    //***********************************************
    virtual void Sync_DataReaource_To_Memory() {};

    //***********************************************
    //定时同步数据源和缓冲中的数据，需要继承子类去实现之(内存 -> 数据源)
    //***********************************************
    virtual void Sync_Memory_To_DataReaource() {};

protected:
    //***********************************************
    //当缓冲不存在，第一次初始化缓冲，需要继承子类去实现之
    //***********************************************
    virtual bool Read_All_Init_DataResoure()
    {
        return true;
    }

    //***********************************************
    //当缓冲存在，从缓冲中还原对应关系，需要继承子类去实现之
    //***********************************************
    virtual bool Read_All_From_CacheMemory()
    {
        return true;
    }

    //***********************************************
    //开始同步的一些初始化动作
    //***********************************************
    virtual void Begin_Sync_DataReaource_To_Memory() {};

    //***********************************************
    //同步完成后的一些动作
    //***********************************************
    virtual void End_Sync_DataReaource_To_Memory() {};

    //***********************************************
    //设置缓冲个数
    //***********************************************
    void Set_Cache_Count(uint32 u4CacheCount)
    {
        m_u4Count = u4CacheCount;
    };

    //***********************************************
    //得到当前缓冲个数
    //***********************************************
    uint32 Get_Cache_Count()
    {
        return m_u4Count;
    };

    //***********************************************
    //得到指定位置的缓冲数据
    //***********************************************
    _CacheBlock* Get_CacheBlock_By_Index(uint32 u4Index)
    {
        return (_CacheBlock* )m_SMOption.GetBuffer(u4Index);
    };

    //***********************************************
    //得到指定位置头维护信息
    //***********************************************
    _SMHeader* Get_SMHead_By_Index(uint32 u4Index)
    {
        return m_SMOption.GetHeader(u4Index);
    };

    //***********************************************
    //设置缓冲区全部加载成功标记位
    //***********************************************
    void Set_Memory_Init_Success()
    {
        m_SMOption.SetMemoryState(READERINITSTATED);
    };

private:
    CSMOption    m_SMOption;
    uint32       m_u4Count;

};
#endif

