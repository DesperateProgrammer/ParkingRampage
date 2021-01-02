#include "gameclass.h"

typedef class CLEVELWONSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
    
    bool m_isHighscore = false ;
        
  public:
    CLEVELWONSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnLeave()  ;
    virtual bool OnTick() ;  

} CLEVELWONSTATE ;