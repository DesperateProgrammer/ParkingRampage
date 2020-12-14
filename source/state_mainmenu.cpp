#include "gameclass.h"
#include <nds.h>
#include <stdio.h>

void CGAME::MainMenu_EnterState()
{
  DisableAllSprites();
  m_subText->Clear() ;
  m_mainText->Clear() ;
  StartFade(1, eFADEIN, MAINMENU_FADETIME) ;
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
}

bool CGAME::MainMenu_Tick()
{
  ShowMainMenuSelector();
  ShowMainMenuStart() ;
  ShowMainMenuMusicVolume();
  uint16_t keys = m_keysDown ;
  for (unsigned int key=0;key<sizeof(keys)*8;key++)
  {
    if (keys & (1 << key))
    {
      switch (1 << key)
      {
        case KEY_TOUCH:
          {
            uint8_t lineTimes2 = m_touch.py / 8 ;
            if ((lineTimes2 > 3) && (lineTimes2 < 3 + MAINMENUITEM_COUNT*4))
            {
              uint8_t item = (lineTimes2-3) / 4 ;
              if (m_mainMenuItem == item)
              {
                // modify item
                switch (m_mainMenuItem)
                {
                  case MAINMENUITEM_DIFFICULTY:
                    IncreaseDifficulty();
                    ShowMainMenuDifficulty();
                    break;
                  case MAINMENUITEM_VOLUME:
                    if (m_touch.px < 113 + m_musicVolume/16)
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
                    m_audio->SetMusicVolume(m_musicVolume) ;
                    break ;
                  case MAINMENUITEM_START:
                    StartLevel() ;
                    return true ;
                  case MAINMENUITEM_SELECT:
                    ChangeState(GAMESTATE_LEVELSELECT) ;
                    return true;
                }
              } else 
              {
                // select item
                m_mainMenuItem = item ;
                break;
              }
            }
          }
          break ;
        case KEY_LEFT:
        case KEY_B:
        case KEY_SELECT:
          switch (m_mainMenuItem)
          {
            case MAINMENUITEM_DIFFICULTY:
              DecreaseDifficulty();
              ShowMainMenuDifficulty();
              break;
            case MAINMENUITEM_VOLUME:
              m_musicVolume -= 64 ;
              if ((m_musicVolume > 1024) || (m_musicVolume < 0))
                m_musicVolume = 0 ;
              m_audio->SetMusicVolume(m_musicVolume) ;
              break ;
            case MAINMENUITEM_START:
              StartLevel() ;
              return true ;
            case MAINMENUITEM_SELECT:
              ChangeState(GAMESTATE_LEVELSELECT) ;
              return true;
          }
          break;
        case KEY_RIGHT:
        case KEY_A:
        case KEY_START:
          switch (m_mainMenuItem)
          {
            case MAINMENUITEM_DIFFICULTY:
              IncreaseDifficulty();
              ShowMainMenuDifficulty();
              break;
            case MAINMENUITEM_START:
              StartLevel() ;
              return true ;
            case MAINMENUITEM_VOLUME:
              m_musicVolume += 64 ;
              if ((m_musicVolume > 1024) || (m_musicVolume < 0))
                m_musicVolume = 1024 ;
              m_audio->SetMusicVolume(m_musicVolume) ;
              break ;
            case MAINMENUITEM_SELECT:
              ChangeState(GAMESTATE_LEVELSELECT) ;
              return true;
          }
          break;
        case KEY_UP:
          m_mainMenuItem = (m_mainMenuItem - 1 + MAINMENUITEM_COUNT) % MAINMENUITEM_COUNT ;
          ShowMainMenuSelector() ;
          break;
        case KEY_DOWN:
          m_mainMenuItem = (m_mainMenuItem + 1) % MAINMENUITEM_COUNT ;
          ShowMainMenuSelector() ;
          break;
        default:
          break;
      }
    }
  }
  return true ;
}

void CGAME::ShowMainMenuMusicVolume() 
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
  m_subText->SetText(10, 2, buffer) ;
}

void CGAME::ShowMainMenuDifficulty()
{
  char *diffText = 0;
  uint8_t pal = 0 ;
  switch (m_difficulty)
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
  m_subText->SetBiColorText(16 - strlen(diffText) / 2, 4, diffText, pal, pal+1) ;
}

void CGAME::ShowMainMenuStart()
{
  char *text = (char *)"Random level" ;
  m_subText->SetText(16 - strlen(text) / 2, 6, text) ;
  text = (char *)"Select level" ;
  m_subText->SetText(16 - strlen(text) / 2, 8, text) ;
}

void CGAME::ShowMainMenuSelector()
{
  int32_t scale = sinLerp(GetTimerTicks()*70);
  oamSet(&oamSub, 0, 2*2*8 + scale * 8 / 4096, 2*2*8 - 25 + 4*8*m_mainMenuItem, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
        m_spriteContent[1], 0, true, false, false, false, false);
  oamSet(&oamSub, 1, 12*2*8 - scale * 8 / 4096, 2*2*8 - 25 + 4*8*m_mainMenuItem, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
        m_spriteContent[1], 0, true, false, false, false, false);
  oamUpdate(&oamSub);  
}
