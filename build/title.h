
//{{BLOCK(title)

//======================================================================
//
//	title, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 178 tiles (t|f reduced) not compressed
//	+ affine map, not compressed, 32x24 
//	Total size: 512 + 11392 + 768 = 12672
//
//	Time-stamp: 2020-12-15, 13:09:18
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLE_H
#define GRIT_TITLE_H

#define titleTilesLen 11392
extern const unsigned int titleTiles[2848];

#define titleMapLen 768
extern const unsigned short titleMap[384];

#define titlePalLen 512
extern const unsigned short titlePal[256];

#endif // GRIT_TITLE_H

//}}BLOCK(title)
