#include "gameclass.h"
#include "tilemap.h"
#include <nds.h>

typedef class CCONTROLMENUSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
    
    CTILEMAP *m_menuTiles ;
    
    
    uint8_t m_menuItem = 0 ;

    void ShowMenuSelector() ;    
        
  public:
    CCONTROLMENUSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnLeave()  ;
    virtual bool OnTick() ;  

} CCONTROLMENUSTATE ;