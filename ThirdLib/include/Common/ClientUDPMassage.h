#ifndef _CLIENTUDPMESSAGE_H
#define _CLIENTUDPMESSAGE_H

//����UDP�ͻ������ݽ��գ���������ͨѶ
//UDP�����������ӵģ�Ҳ���ش������ݰ��ľ����и����������ȫ���԰��յ������ݽ����û��Լ�����
//add by freeeyes


class IClientUDPMessage
{
public:
    virtual ~IClientUDPMessage() {}
    virtual bool RecvData(const char* pData, int nLen, _ClientIPInfo objServerIPInfo) = 0;    //�������ݵĺ���
};

#endif
