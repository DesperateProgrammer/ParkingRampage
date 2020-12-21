#include "gameclass.h"

typedef class CLEVELLOADINGSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
        
  public:
    CLEVELLOADINGSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnTick() ;  

} CLEVELLOADINGSTATE ;