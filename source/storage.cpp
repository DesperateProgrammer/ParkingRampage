#include "storage.h"
#include <nds.h>

extern char **gArgV ;

class CSTORAGE *CSTORAGE::m_self = 0;

CSTORAGE::CSTORAGE() 
{
  m_type = eNONE ;
  if (fatInitDefault())
  {
    m_type = eFAT;
  }
}

class CSTORAGE *CSTORAGE::GetInstance() 
{
  if (!m_self)
    m_self = new CSTORAGE() ;
  return m_self ;
}
