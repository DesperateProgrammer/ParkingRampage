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

uint32_t CTLV::GetChildCount()
{
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    return m_childs.size() ;
  } else
  {
    return 0 ;
  }
}

CTLV *CTLV::GetChild(uint32_t index) 
{
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    if (index >= m_childs.size())
      return 0 ;
    return m_childs[index] ;
  } else
  {
    return 0 ;
  }
}

void CTLV::SetUInt32(uint32_t value) 
{
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    return ;
  }
  char buffer[4] ;
  for (int i=0;i<4;i++)
  {
    buffer[i] = (value >> (i*8)) & 0xff ;
  }
  m_leaf = std::string(buffer, 4) ;
}

uint32_t CTLV::GetUInt32() 
{
  if (m_tag & TAG_FLAG_RECURSIVE)
  {
    return 0;
  }
  if (m_leaf.length() < 4)
  {
    return 0;
  }
  uint32_t tmp = 0 ;
  for (int i=0;i<4;i++)
  {
    tmp |= ((uint32_t)m_leaf[i]) << (i*8) ;
  }
  return tmp ;
}