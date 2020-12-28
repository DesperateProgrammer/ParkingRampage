#pragma once

#include <fat.h>
#include <stdio.h>
#include <string>

typedef enum ESTORAGETYPE
{
  eNONE,
  eFAT,
  eNITRO
} ESTORAGETYPE ;

typedef class CSTORAGE
{
  protected:
    static class CSTORAGE *m_self ;
    
    ESTORAGETYPE  m_type ;
    
    CSTORAGE() ;
  public:
    class CSTORAGE *GetInstance() ;
    
    
} CSTORAGE ;
