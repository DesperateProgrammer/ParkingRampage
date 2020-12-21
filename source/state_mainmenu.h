#include "gameclass.h"
#include <nds.h>

#define MAINMENUITEM_VOLUME       0
#define MAINMENUITEM_DIFFICULTY   1
#define MAINMENUITEM_START        2
#define MAINMENUITEM_SELECT       3
#define MAINMENUITEM_COUNT        4


typedef class CMAINMENUSTATE : public CSTATE
{
  protected:
    CGAME *m_game ;
    
    CTILEMAP *m_mainMenuTiles ;
    
    uint16_t m_musicVolume = 768 ;
    
    uint8_t m_mainMenuItem = MAINMENUITEM_START ;

    void ShowMainMenuDifficulty() ;
    void ShowMainMenuStart() ;
    void ShowMainMenuSelector() ;
    void ShowMainMenuMusicVolume() ;
    
        
  public:
    CMAINMENUSTATE(CGAME *game) ;
  
    virtual bool OnEnter()  ;
    virtual bool OnLeave()  ;
    virtual bool OnTick() ;  

} CMAINMENUSTATE ;