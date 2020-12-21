#include "gameclass.h"

typedef class CLEVELPAUSEDSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
        
  public:
    CLEVELPAUSEDSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnLeave()  ;
    virtual bool OnTick() ;  

} CLEVELPAUSEDSTATE ;