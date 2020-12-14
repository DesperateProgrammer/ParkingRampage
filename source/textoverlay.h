#pragma once

#include <stdint.h>

/* 
  Textoverlay uses BG0 on Main or Sub Screen
  to display text in an 8x16 pixel font. 
  
  A screen contains 32x12 characters
*/

#define TEXTCOLOR_WHITE               0
#define TEXTCOLOR_RED                 1
#define TEXTCOLOR_DARKRED             2
#define TEXTCOLOR_GREEN               3
#define TEXTCOLOR_DARKGREEN           4
#define TEXTCOLOR_BLUE                5
#define TEXTCOLOR_DARKBLUE            6
#define TEXTCOLOR_YELLOW              7
#define TEXTCOLOR_DARKYELLOW          8
#define TEXTCOLOR_RAINBOW             9


typedef class CTEXTOVERLAY
{
  protected:
    const uint8_t m_screen ;
    const uint16_t *m_tileDataSource ;
    const uint16_t *m_tilePalSource ;
    const uint16_t *m_tileMapSource ;
    const uint32_t m_dataLength, m_paletteLength, m_mapLength ;
    
    static void RainbowVSyncHandler() ;
    
  public:
    CTEXTOVERLAY(const uint8_t screen); 
    
    void Initialize() ;
    void Dispose() ;
    
    void SetChar(uint8_t x, uint8_t y, char ch, uint8_t pal = 0) ;
    void SetText(uint8_t x, uint8_t y, char *text, uint8_t pal = 0) ;
    
    void SetBiColorChar(uint8_t x, uint8_t y, char ch, uint8_t pal1 = 0, uint8_t pal2 = 0) ;
    void SetBiColorText(uint8_t x, uint8_t y, char *text, uint8_t pal1 = 0, uint8_t pal2 = 0) ;
    
    void SetBorder(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t palette = 0) ;
    
    /* put a window over the text (in gfx coordinates) */
    void EnableTextWindow(uint8_t x, uint8_t y, uint8_t width, uint8_t height) ;
    void DisableTextwindow() ;
    
    void Clear() ;
} CTEXTOVERLAY ;
