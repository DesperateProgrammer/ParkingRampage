#pragma once

#include "tlv.h"
#include <fat.h>
#include <stdio.h>
#include <string>

#define CONFIG_ROOT     ((unsigned long)(TAG_FLAG_RECURSIVE | (uint32_t)0x474643))



typedef class CSTORAGE
{
  protected:
    static class CSTORAGE *m_self ;
    
    CSTORAGE() ;
    
    std::string m_basename;
    
    CTLV *m_config = 0;
    
  public:
    static class CSTORAGE *GetInstance() ;
    
    CTLV *GetConfig(uint32_t tag) ;
    
    void Flush() ;
    
    
} CSTORAGE ;
