#include "tlv.h"

CTLV::CTLV(uint32_t tag)
{
  m_tag = tag ;
  if (tag & TAG_FLAG_RECURSIVE)
  {
    m_childs.clear();
  } else
  {
    m_leaf = "";
  }
}

CTLV::CTLV(std::string serializedTLV)
{
  m_tag = 0 ;
  uint32_t length = 0 ;
  if (serializedTLV.length() < 4)
  {
    m_tag = 0 ;
    m_leaf = "";
    return ;
  }
  for (int i=0;i<4;i++)
    m_tag |= (uint32_t)serializedTLV[i] << (i*8) ;
  if (serializedTLV.length() < 4)
  {
    if (m_tag & TAG_FLAG_RECURSIVE)
    {
      m_childs.clear();
    } else
    {
      m_leaf = "";
    }
    return ;
  }
  for (int i=0;i<4;i++)
    length |= (uint32_t)serializedTLV[i+4] << (i*8) ;
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    m_childs.clear() ;
    serializedTLV = serializedTLV.substr(8, length) ;
    uint32_t pos = 0 ;
    while (pos <= length - 8)
    {
      CTLV * child = new CTLV(serializedTLV) ;
      pos += child->Serialize().length() ;
      m_childs.push_back(child) ;
    }
  } else
  {
    m_leaf = serializedTLV.substr(8, length);
  }
}
   
bool CTLV::SetData(uint8_t *src, uint32_t length) 
{
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    return false ;
  } else
  {
    m_leaf = std::string((const char *)src, length);
    return true ;
  }
}

bool CTLV::AddChild(class CTLV *tlv)
{
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    m_childs.push_back(tlv) ;
    return true ;
  } else
  {
    return false ;
  }
}

bool CTLV::RemoveChild(class CTLV *tlv) 
{
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    std::vector<class CTLV *>::iterator i ;
    for (i=m_childs.begin();i!=m_childs.end();i++)
    {
      if ((*i) == tlv)
      {
        m_childs.erase(i) ;
        return true ;
      }
    }
    return false ;
  } else
  {
    return false ;
  }
}
   
std::string CTLV::Serialize() 
{
  std::string tmp = "" ;
  for (int i=0;i<4;i++)
    tmp.append(1, (char)((m_tag >> (i*8)) & 0xFF)) ;
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    std::string childData = "" ;
    for (unsigned int i=0;i<m_childs.size();i++)
    {
      childData += m_childs[i]->Serialize() ;
    }
    for (int i=0;i<4;i++)
      tmp.append(1, (char)((childData.length() >> (i*8)) & 0xFF)) ;
    tmp += childData ;      
  } else
  {
    for (int i=0;i<4;i++)
      tmp.append(1, (char)((m_leaf.length() >> (i*8)) & 0xFF)) ;
    tmp += m_leaf ;  
  }
  return tmp;
}