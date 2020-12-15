
//{{BLOCK(leveltiles)

//======================================================================
//
//	leveltiles, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 13 tiles (t|f reduced) not compressed
//	+ affine map, not compressed, 32x24 
//	Total size: 512 + 832 + 768 = 2112
//
//	Time-stamp: 2020-12-15, 13:09:17
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVELTILES_H
#define GRIT_LEVELTILES_H

#define leveltilesTilesLen 832
extern const unsigned int leveltilesTiles[208];

#define leveltilesMapLen 768
extern const unsigned short leveltilesMap[384];

#define leveltilesPalLen 512
extern const unsigned short leveltilesPal[256];

#endif // GRIT_LEVELTILES_H

//}}BLOCK(leveltiles)
