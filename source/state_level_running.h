#include "gameclass.h"

typedef class CLEVELRUNNINGSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
        
  public:
    CLEVELRUNNINGSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnTick() ;  

} CLEVELRUNNINGSTATE ;