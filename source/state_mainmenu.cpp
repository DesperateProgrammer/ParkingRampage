#include "gameclass.h"
#include <nds.h>
#include <stdio.h>
#include "state_mainmenu.h"
#include "mainmenu.h"

CMAINMENUSTATE::CMAINMENUSTATE(CGAME *game) 
{
  m_game = game ;
  m_game->GetAudio()->SetMusicVolume(m_musicVolume) ;
  m_mainMenuTiles = new CTILEMAP(1, (uint16_t *)mainmenuTiles, mainmenuTilesLen, (uint16_t *)mainmenuPal, mainmenuPalLen, (uint16_t *)mainmenuMap, mainmenuMapLen) ;
}

bool CMAINMENUSTATE::OnEnter()
{
  m_game->StartFade(1, eFADEIN, MAINMENU_FADETIME) ;
  m_mainMenuTiles->Initialize() ;        
  for (int i=0;i<16;i++)
  {
    for (int q=0;q<16;q++)
    {
      m_mainMenuTiles->SetMap(i*2,q*2,1) ;
      m_mainMenuTiles->SetMap(i*2+1,q*2,2) ;
      m_mainMenuTiles->SetMap(i*2,q*2+1,3) ;
      m_mainMenuTiles->SetMap(i*2+1,q*2+1,4) ;
    }
  }
  
  ShowMainMenuDifficulty();
  ShowMainMenuStart() ;
  ShowMainMenuSelector() ;
  return true ;
}

bool CMAINMENUSTATE::OnLeave()
{
  m_game->GetSubText()->Clear() ;
  m_game->GetMainText()->Clear() ;
  m_game->DisableAllSprites() ;
  return true ;
}

bool CMAINMENUSTATE::OnTick()
{
  ShowMainMenuSelector();
  ShowMainMenuStart() ;
  ShowMainMenuMusicVolume();
  uint16_t keys = m_game->GetInputManager()->GetKeysDown() ; ;
  if (keys & KEY_TOUCH)
  {
    uint8_t lineTimes2 = m_game->GetInputManager()->GetLastTouchDownPosition().py / 8 ;
    if ((lineTimes2 > 3) && (lineTimes2 < 3 + MAINMENUITEM_COUNT*4))
    {
      uint8_t item = (lineTimes2-3) / 4 ;
      if (m_mainMenuItem == item)
      {
        // modify item
        switch (m_mainMenuItem)
        {
          case MAINMENUITEM_DIFFICULTY:
            m_game->IncreaseDifficulty();
            ShowMainMenuDifficulty();
            break;
          case MAINMENUITEM_VOLUME:
            if (m_game->GetInputManager()->GetLastTouchDownPosition().px < 113 + m_musicVolume/16)
            {
              m_musicVolume -= 64 ;
              if ((m_musicVolume > 1024) || (m_musicVolume < 0))
                m_musicVolume = 0 ;
            } else
            {
              m_musicVolume += 64 ;
              if ((m_musicVolume > 1024) || (m_musicVolume < 0))
                m_musicVolume = 1024 ;
            }
            m_game->GetAudio()->SetMusicVolume(m_musicVolume) ;
            break ;
          case MAINMENUITEM_START:
            m_game->GetLevelManager()->UnloadLevel() ;
            m_game->StartFade(SCREEN_BOTTOM, eFADEOUT, LEVEL_FADETIME) ;
            m_sm->ChangeState(GAMESTATE_LEVELLOADING);
            return true ;
          case MAINMENUITEM_SELECT:
            m_sm->ChangeState(GAMESTATE_LEVELSELECT) ;
            return true;
        }
      } else 
      {
        // select item
        m_mainMenuItem = item ;
      }
    }
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MENU_MODIFY_MINUS))
  {  
    switch (m_mainMenuItem)
    {
      case MAINMENUITEM_DIFFICULTY:
        m_game->DecreaseDifficulty();
        ShowMainMenuDifficulty();
        break;
      case MAINMENUITEM_VOLUME:
        m_musicVolume -= 64 ;
        if ((m_musicVolume > 1024) || (m_musicVolume < 0))
          m_musicVolume = 0 ;
        m_game->GetAudio()->SetMusicVolume(m_musicVolume) ;
        break ;
      case MAINMENUITEM_START:
        m_game->GetLevelManager()->UnloadLevel() ;
        m_game->StartFade(SCREEN_BOTTOM, eFADEOUT, LEVEL_FADETIME) ;
        m_sm->ChangeState(GAMESTATE_LEVELLOADING);
        return true ;
      case MAINMENUITEM_SELECT:
        m_sm->ChangeState(GAMESTATE_LEVELSELECT) ;
        return true;
    }
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MENU_MODIFY_PLUS))
  {  
    switch (m_mainMenuItem)
    {
      case MAINMENUITEM_DIFFICULTY:
        m_game->IncreaseDifficulty();
        ShowMainMenuDifficulty();
        break;
      case MAINMENUITEM_START:
        m_game->GetLevelManager()->UnloadLevel() ;
        m_game->StartFade(SCREEN_BOTTOM, eFADEOUT, LEVEL_FADETIME) ;
        m_sm->ChangeState(GAMESTATE_LEVELLOADING);
        return true ;
      case MAINMENUITEM_VOLUME:
        m_musicVolume += 64 ;
        if ((m_musicVolume > 1024) || (m_musicVolume < 0))
          m_musicVolume = 1024 ;
        m_game->GetAudio()->SetMusicVolume(m_musicVolume) ;
        break ;
      case MAINMENUITEM_SELECT:
        m_sm->ChangeState(GAMESTATE_LEVELSELECT) ;
        return true;
    }
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MENU_UP))
  {  
    m_mainMenuItem = (m_mainMenuItem - 1 + MAINMENUITEM_COUNT) % MAINMENUITEM_COUNT ;
    ShowMainMenuSelector() ;
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MENU_DOWN))
  {  
    m_mainMenuItem = (m_mainMenuItem + 1) % MAINMENUITEM_COUNT ;
    ShowMainMenuSelector() ;
  }
  return true ;
}

void CMAINMENUSTATE::ShowMainMenuMusicVolume() 
{
  char buffer[] = "\x0D\x0D \x17\x18\x18\x18\x18\x18\x18\x18\x18\x0F";
  for (int i=0;i<m_musicVolume / 128; i++)
  {
    buffer[i+4] = 0x0E ;
  }
  if ((m_musicVolume % 128) / 16)
  {
    buffer[4+m_musicVolume / 128] = 0x18 + ((m_musicVolume % 128) / 16) ;
  }
  m_game->GetSubText()->SetText(10, 2, buffer) ;
}

void CMAINMENUSTATE::ShowMainMenuDifficulty()
{
  char *diffText = 0;
  uint8_t pal = 0 ;
  switch (m_game->GetDifficulty())
  {
    case eBEGINNER:
      diffText = (char *)"  BEGINNER  " ;
      pal = 3 ;
      break ;
    case eINTERMEDIATE:
      diffText = (char *)"INTERMEDIATE" ;
      pal = 5 ;
      break ;
    case eADVANCED:
      diffText = (char *)"  ADVANCED  " ;
      pal = 7 ;
      break ;
    case eEXPERT:
      diffText = (char *)"   EXPERT   " ;
      pal = 1 ;
      break ;
  }
  m_game->GetSubText()->SetBiColorText(16 - strlen(diffText) / 2, 4, diffText, pal, pal+1) ;
}

void CMAINMENUSTATE::ShowMainMenuStart()
{
  char *text = (char *)"Random level" ;
  m_game->GetSubText()->SetText(16 - strlen(text) / 2, 6, text) ;
  text = (char *)"Select level" ;
  m_game->GetSubText()->SetText(16 - strlen(text) / 2, 8, text) ;
}

void CMAINMENUSTATE::ShowMainMenuSelector()
{
  int32_t scale = sinLerp(m_game->GetTimeManager()->GetTimerTicks()*70);
  oamSet(&oamSub, 0, 2*2*8 + scale * 8 / 4096, 2*2*8 - 25 + 4*8*m_mainMenuItem, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
        m_game->GetSpriteLocation(SPRITE_CAR_TARGET), 0, true, false, false, false, false);
  oamSet(&oamSub, 1, 12*2*8 - scale * 8 / 4096, 2*2*8 - 25 + 4*8*m_mainMenuItem, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
        m_game->GetSpriteLocation(SPRITE_CAR_TARGET), 0, true, false, false, false, false);
  oamUpdate(&oamSub);  
}
