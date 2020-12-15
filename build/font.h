
//{{BLOCK(font)

//======================================================================
//
//	font, 128x128@8, 
//	+ palette 256 entries, not compressed
//	+ 257 tiles not compressed
//	+ regular map (flat), not compressed, 16x16 
//	Total size: 512 + 16448 + 512 = 17472
//
//	Time-stamp: 2020-12-15, 13:09:17
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FONT_H
#define GRIT_FONT_H

#define fontTilesLen 16448
extern const unsigned int fontTiles[4112];

#define fontMapLen 512
extern const unsigned short fontMap[256];

#define fontPalLen 512
extern const unsigned short fontPal[256];

#endif // GRIT_FONT_H

//}}BLOCK(font)
