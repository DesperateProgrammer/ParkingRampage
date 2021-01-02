#pragma once

#include "gamebase.h"
#include "menuitem.h"
#include "menuitem_coloredenumeration.h"
#include "menuitem_statechange.h"
#include <vector>

typedef class CMENU
{
  protected:
    std::vector<CMENUITEM *>  m_items ;
    uint32_t                  m_selection ;
    uint32_t                  m_firstVisible ;
    class CGAMEBASE *         m_game ;
        
  public:
    CMENU(class CGAMEBASE *game) ;
    
    virtual ~CMENU() {} ;

    
    void AddItem(CMENUITEM *item) ;
    void RemoveItem(CMENUITEM *item) ;
    
    CMENUITEM *GetSelectedItem() ;
    
    virtual void OnTick() ;
    virtual void OnItemSelected(CMENUITEM *item) ;
    virtual void OnItemModified(CMENUITEM *item) ;
    virtual void OnDrawItemSelection(uint8_t y) ;
    
    void SelectNext() ;
    void SelectPrevious() ;
    void SelectItem(CMENUITEM *item) ;
    
    virtual CMENUITEM *GetItemFromCoordinates(uint8_t x, uint8_t y) ;
    
} CMENU ;