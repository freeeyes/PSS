#include "PacketParse.h"

void CPacketParse::SetHashID(int nHashID)
{
    m_nHashID = nHashID;
}

int CPacketParse::GetHashID() const
{
    return m_nHashID;
}

void CPacketParse::Init()
{
    m_nHashID = 0;
}

