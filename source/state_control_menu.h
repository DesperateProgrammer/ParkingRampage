#include "gameclass.h"
#include "tilemap.h"
#include "menu.h"
#include <nds.h>

typedef class CCONTROLMENUSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
    
    CTILEMAP *m_mainMenuTiles ;
        
    CMENU *m_menu ;
    CMENUITEM *m_menuitems[4] ; 
        
  public:
    CCONTROLMENUSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnLeave()  ;
    virtual bool OnTick() ;  

} CCONTROLMENUSTATE ;