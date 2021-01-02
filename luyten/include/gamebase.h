#pragma once

#include "textoverlay.h"
#include "audiostreaming.h"
#include "inputmanager.h"
#include "timemanager.h"
#include "spritemanager.h"
#include "statemachine.h"
#include <string>
#include <stdint.h>

typedef enum EFADE
{
  eFADEIN,
  eFADEOUT,
} EFADE;

typedef class CGAMEBASE : public CSTATEMACHINE
{
  protected:
    /* Singleton */
    static class CGAMEBASE *m_instance ;

    /* State machine */
    virtual void Initialize() ;
    virtual void Destruct() ; 
    virtual void OnEveryTick() ;
            
    
    /* Various managers */
    /* Time */
    CTIMEMANAGER *m_time ;
    /* Input (Key + Touch) */
    CINPUTMANAGER m_input ;
    /* Audio */
    CAUDIOSTREAMING *m_audio ;        
    /* Text background */
    CTEXTOVERLAY *m_mainText ;
    CTEXTOVERLAY *m_subText ;
    /* Sprites */
    CSPRITEMANAGER *m_mainSprites ;
    CSPRITEMANAGER *m_subSprites ;
    
    /* Effects */
    /* Fading state for both screens */
    uint32_t m_fadeStart[2];
    uint32_t m_fadeEnd[2];
    EFADE m_fadeMode[2] ;
    
    void UpdateFading(uint8_t screen) ;

        
    /* Rotate out/Rotate in */
    uint32_t m_rotscaleStart[2];
    uint32_t m_rotscaleEnd[2];
    // The following rotscale values are 4.12 fixed point
    uint16_t m_startScale[2] ;
    uint16_t m_endscale[2] ;
    int16_t m_rotations[2] ;

    void UpdateRotScale(uint8_t screen) ;
    
    std::string m_nickname ;
    
  public:
    /* Singleton */
    static class CGAMEBASE *GetInstance() { return m_instance; };

    /* State Machine */
    void Run();
    
    /* Get Various Managers */
    CINPUTMANAGER *GetInputManager() { return &m_input ; } ;
    CAUDIOSTREAMING * GetAudio() { return m_audio; };
    CTEXTOVERLAY *GetMainText() { return m_mainText ;} ;
    CTEXTOVERLAY *GetSubText() { return m_subText ;} ;
    CTIMEMANAGER *GetTimeManager() { return m_time; };    
    CSPRITEMANAGER *GetMainSprites() { return m_mainSprites ; };
    CSPRITEMANAGER *GetSubSprites() { return m_subSprites ; };

    
    /* Effects */
    /* Fading state for both screens */
    void StartFade(uint8_t screen, EFADE mode, uint32_t timespan) ;
    EFADE GetFadeMode(uint8_t screen) ;
    bool IsFading(uint8_t screen) ;        
    /* Rotate out/Rotate in */
    void StartRotScale(uint8_t screen, uint32_t timespan, int16_t rotations, uint16_t startScale, uint16_t endScale) ;
    bool IsRotScaling(uint8_t screen) ;
    
    std::string GetNickname() { return m_nickname; };
    
} CGAMEBASE, LPCGAMEBASE;