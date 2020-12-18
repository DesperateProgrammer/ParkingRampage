#pragma once

#include "defaultKeyMapping.h"
#include <stdint.h>
#include <vector>
#include <map>
#include <nds.h>

#define INPUTEVENT_DOWN       1
#define INPUTEVENT_HELD       2
#define INPUTEVENT_RELEASED   4

using namespace std;

typedef void (*keyhandler)(uint16_t key, void *context, uint8_t event) ;

typedef struct SINPUTHANDLER
{
  keyhandler  handler ;
  void *      context ;
  uint16_t    filter ;
} SINPUTHANDLER ;

typedef class CINPUTMANAGER
{
  protected:
    uint16_t m_keysReleased = 0;
    uint16_t m_keysDown = 0;
    uint16_t m_keysHeld = 0;
    
    map<uint16_t, uint16_t> m_mapping ;
    
    touchPosition m_touch;
    touchPosition m_touchDown;
    
    vector<SINPUTHANDLER> m_registeredDownHandlers ;
    vector<SINPUTHANDLER> m_registeredUpHandlers ;
    
    void OnKeyDown(uint16_t key) ;
    void OnKeyReleased(uint16_t key) ;
    
  public:
    CINPUTMANAGER() ;
  
    void Tick() ;
    
    uint16_t GetKeysDown() ;
    uint16_t GetKeysReleased() ;
    uint16_t GetKeysHeld() ;
    
    touchPosition GetLastTouchPosition() ;
    touchPosition GetLastTouchDownPosition() ;
    
    void AccountDrag() ;
    
    void AddHandler(uint8_t eventType, uint16_t filter, keyhandler handler, void *context) ;
    void RemoveHandler(uint8_t eventType, uint16_t filter, keyhandler handler, void *context) ;
    
    void EnumerateKeysDown(keyhandler handler, void *context) ;
    void EnumerateKeysRelease(keyhandler handler, void *context) ;
    void EnumerateKeysHeld(keyhandler handler, void *context) ;
    
    void SetKeyMapping(uint16_t alias, uint16_t keys) ;
    uint16_t GetKeyMapping(uint16_t alias) ;
    bool IsKeyForAlias(uint16_t key, uint16_t alias) ;
    

} CINPUTMANAGER ;