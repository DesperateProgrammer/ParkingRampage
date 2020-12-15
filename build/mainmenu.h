
//{{BLOCK(mainmenu)

//======================================================================
//
//	mainmenu, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 102 tiles (t|f reduced) not compressed
//	+ affine map, not compressed, 32x24 
//	Total size: 512 + 6528 + 768 = 7808
//
//	Time-stamp: 2020-12-15, 13:09:17
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAINMENU_H
#define GRIT_MAINMENU_H

#define mainmenuTilesLen 6528
extern const unsigned int mainmenuTiles[1632];

#define mainmenuMapLen 768
extern const unsigned short mainmenuMap[384];

#define mainmenuPalLen 512
extern const unsigned short mainmenuPal[256];

#endif // GRIT_MAINMENU_H

//}}BLOCK(mainmenu)
