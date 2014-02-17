#include "StdAfx.h"
#include "CommandInfo.h"

//全局函数，用于排序CommandInfo
bool CompLess(const _CommaindInfo* lhs, const _CommaindInfo* rhs)
{
  return lhs->m_nCommandSort < rhs->m_nCommandSort;
}

CCommandInfo::CCommandInfo(void)
{
}

CCommandInfo::~CCommandInfo(void)
{
  Close();
}

void CCommandInfo::SetCommandData( const char* pCommandID, int nCommandGroup, int nCommandSort, const char* pSendData )
{
  mapCommaindInfoGroup::iterator f = m_mapCommaindInfoGroup.find(nCommandGroup);
  if(f != m_mapCommaindInfoGroup.end())
  {
    vecCommaindInfo* pvecCommaindInfo = (vecCommaindInfo* )f->second;
    if(pvecCommaindInfo != NULL)
    {
      _CommaindInfo* pCommaindInfo = new _CommaindInfo();
      pCommaindInfo->SetData(pCommandID, nCommandGroup, nCommandSort, pSendData);
      pvecCommaindInfo->push_back(pCommaindInfo);
    }
  }
  else
  {
    vecCommaindInfo* pvecCommaindInfo = new vecCommaindInfo();
    _CommaindInfo* pCommaindInfo = new _CommaindInfo();
    pCommaindInfo->SetData(pCommandID, nCommandGroup, nCommandSort, pSendData);
    pvecCommaindInfo->push_back(pCommaindInfo);
    m_mapCommaindInfoGroup.insert(mapCommaindInfoGroup::value_type(nCommandGroup, pvecCommaindInfo));
  }
}

void CCommandInfo::Close()
{
  for(mapCommaindInfoGroup::iterator b = m_mapCommaindInfoGroup.begin(); b != m_mapCommaindInfoGroup.end(); b++)
  {
    vecCommaindInfo* pvecCommaindInfo = (vecCommaindInfo* )b->second;
    if(NULL != pvecCommaindInfo)
    {
      for(int i = 0; i < (int)pvecCommaindInfo->size(); i++)
      {
        _CommaindInfo* pCommaindInfo = (_CommaindInfo* )(*pvecCommaindInfo)[i];
        delete pCommaindInfo;
        pCommaindInfo = NULL;
      }

      pvecCommaindInfo->clear();
      delete pvecCommaindInfo;
      pvecCommaindInfo = NULL;
    }
  }
}

int CCommandInfo::GetCommandGroupCount()
{
  return (int)m_mapCommaindInfoGroup.size();
}

vecCommaindInfo* CCommandInfo::GetCommandGroup( int nIndex )
{
  int nPos = 0;
  for(mapCommaindInfoGroup::iterator b = m_mapCommaindInfoGroup.begin(); b != m_mapCommaindInfoGroup.end(); b++)
  {
    if(nPos == nIndex)
    {
      return (vecCommaindInfo* )b->second;
    }
    nPos++;
  }

  return NULL;
}

void CCommandInfo::Sort()
{
  for(mapCommaindInfoGroup::iterator b = m_mapCommaindInfoGroup.begin(); b != m_mapCommaindInfoGroup.end(); b++)
  {
    vecCommaindInfo* pvecCommaindInfo = (vecCommaindInfo* )b->second;
    if(pvecCommaindInfo != NULL)
    {
      sort(pvecCommaindInfo->begin(), pvecCommaindInfo->end(), CompLess);
    }
  }
}
