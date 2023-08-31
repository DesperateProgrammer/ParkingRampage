#include "gameclass.h"
#include "extendedbg.h"
#include <nds.h>

typedef class CGAMELOADINGSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
    
    CTILEMAP *m_mainScreenTiles ;
    CEXTENDEDBG *m_mainScreenBG ;
    CTILEMAP *m_titleTiles ;
    
  public:
    CGAMELOADINGSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnTick() ;  

} CGAMELOADINGSTATE ;