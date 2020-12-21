#include "gameclass.h"

typedef class CLEVELSELECTSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
    
    uint16_t m_levelIndexInDifficulty ;

    void UpdateLevelSelectionInfo() ;
        
  public:
    CLEVELSELECTSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnLeave()  ;
    virtual bool OnTick() ;  

} CLEVELSELECTSTATE ;