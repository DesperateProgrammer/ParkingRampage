#include "storage.h"
#include <nds.h>

class CSTORAGE *CSTORAGE::m_self = 0;

CSTORAGE::CSTORAGE() 
{
  fatInitDefault() ;
  if (__system_argv->argc)
  {
    m_basename = std::string(__system_argv->argv[0]) ;
    m_basename = m_basename.substr(0, m_basename.rfind(".")) ;
    FILE *tlvFile = fopen((m_basename+".cfg").c_str(), "rb+") ;
    if (tlvFile)
    {
        uint32_t size = 0 ;
        fseek(tlvFile, 0, SEEK_END) ;
        size = ftell(tlvFile) ;
        fseek(tlvFile, 0, SEEK_SET) ;
        char *buffer = new char[size]() ;
        fread(buffer, 1, size, tlvFile) ;
        fclose(tlvFile) ;
        m_config = new CTLV(std::string(buffer, size)) ;
        if (!m_config->GetTag())
        {
          m_config = new CTLV(CONFIG_ROOT) ;
        }
    } else
    {
      m_config = new CTLV(CONFIG_ROOT) ;
    }
  }
}

class CSTORAGE *CSTORAGE::GetInstance() 
{
  if (!m_self)
    m_self = new CSTORAGE() ;
  return m_self ;
}

CTLV *CSTORAGE::GetConfig(uint32_t tag) 
{
  for (unsigned int i=0;i<m_config->GetChildCount();i++)
  {
    CTLV *e = m_config->GetChild(i) ;
    if (e->GetTag() == tag)
    {
      return e ;
    }
  }
  CTLV* e = new CTLV(tag) ;
  m_config->AddChild(e) ;
  return e ;
}

void CSTORAGE::Flush() 
{
  std::string data = m_config->Serialize() ;
  FILE *tlvFile = fopen((m_basename+".cfg").c_str(), "wb+") ;
  if (tlvFile)
  {
    fwrite(data.c_str(), 1, data.length(), tlvFile) ;
    fclose(tlvFile) ;
  }
}