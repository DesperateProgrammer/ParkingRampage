
@{{BLOCK(leveltiles)

@=======================================================================
@
@	leveltiles, 256x192@8, 
@	+ palette 256 entries, not compressed
@	+ 13 tiles (t|f reduced) not compressed
@	+ affine map, not compressed, 32x24 
@	Total size: 512 + 832 + 768 = 2112
@
@	Time-stamp: 2020-12-15, 13:09:17
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global leveltilesTiles		@ 832 unsigned chars
	.hidden leveltilesTiles
leveltilesTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x15292518,0x26171818,0x25070623,0x15151818,0x18260320,0x242A2E11,0x0E1A1A25,0x20211520
	.word 0x19031A19,0x251A151F,0x1A1E1507,0x05202520,0x15291512,0x262B1520,0x15151115,0x20061516
	.word 0x20341A1C,0x18151A20,0x15111620,0x1818161A,0x26151815,0x18151519,0x18201D01,0x15200B26
	.word 0x18292005,0x190E1615,0x18191826,0x250E3119,0x15262015,0x11073119,0x190C1907,0x26191519
	.word 0x260E2015,0x05182016,0x30181519,0x20191820,0x26192A2A,0x22052A25,0x2625192A,0x1520160A
	.word 0x13151F19,0x16190A05,0x20261819,0x2E201926,0x1919291A,0x22262629,0x26292513,0x05110E11

	.word 0x191A1519,0x201A1A20,0x14202019,0x2B152515,0x141A2618,0x2A25252E,0x18300315,0x16052520
	.word 0x1C261518,0x18051515,0x0E291825,0x19011929,0x291A2015,0x18192A18,0x15201925,0x182E1515
	.word 0x04040833,0x28282828,0x08092D09,0x2F2F0933,0x33040404,0x0810272F,0x09090908,0x04320404
	.word 0x0404092C,0x08092F2D,0x09080909,0x0804082C,0x04040904,0x2F270F04,0x2F09042F,0x04000904
	.word 0x041B0909,0x28282828,0x2F2F0404,0x09090409,0x2F2F092F,0x332F092C,0x09040833,0x2F042F04
	.word 0x0904042C,0x2C09042F,0x092C092F,0x2809092C,0x2F04042F,0x2809090D,0x2C2C2C09,0x282C2F2C
	.word 0x04090409,0x08090404,0x2D332F2C,0x042C0904,0x042C0808,0x042C1008,0x04082C08,0x092C0410
	.word 0x082F2D2C,0x042C102C,0x042F330D,0x272C2C04,0x2C2C2F09,0x04040404,0x04040808,0x2C2F092F

	.word 0x2C09042C,0x2809092C,0x2F04042C,0x27090809,0x09090433,0x08080810,0x33332F2F,0x042C0804
	.word 0x092F0909,0x092C2F2F,0x09043308,0x28332C04,0x04090409,0x022C0809,0x09042C08,0x28270904
	.word 0x04040833,0x2F2C0909,0x08092D09,0x2F2F0933,0x33040404,0x0810272F,0x09090908,0x04320404
	.word 0x0404092C,0x08092F2D,0x09080909,0x0804082C,0x04040904,0x2F270F04,0x2F09042F,0x04000904
	.word 0x041B0909,0x09090904,0x2F2F0404,0x09090409,0x2F2F092F,0x332F092C,0x09040833,0x2F042F04
	.word 0x0904042C,0x2C09042F,0x092C092F,0x0809092C,0x2F04042F,0x0409090D,0x2C2C2C09,0x2F2C2F2C
	.word 0x04090409,0x08090404,0x2D332F2C,0x042C0904,0x042C0808,0x0804082C,0x04082C08,0x2F270F04
	.word 0x082F2D2C,0x04000904,0x042F330D,0x272C2C04,0x2C2C2F09,0x04040404,0x04040808,0x2C2F092F

	.word 0x2C09042C,0x0409092C,0x2F04042C,0x27090809,0x09090433,0x08080810,0x33332F2F,0x042C0804
	.word 0x092F0909,0x092C2F2F,0x09043308,0x2C332C04,0x04090409,0x332C0809,0x09042C08,0x09270904

	.section .rodata
	.align	2
	.global leveltilesMap		@ 768 unsigned chars
	.hidden leveltilesMap
leveltilesMap:
	.hword 0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201
	.hword 0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0403,0x0403,0x0403,0x0403,0x0403
	.hword 0x0403,0x0403,0x0403,0x0403,0x0403,0x0403,0x0403,0x0403
	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403

	.hword 0x0201,0x0201,0x0201,0x0605,0x0A09,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0C0B,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403
	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403

	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403
	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403

	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403
	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403

	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403
	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403

	.hword 0x0201,0x0201,0x0201,0x0605,0x0605,0x0605,0x0605,0x0605
	.hword 0x0605,0x0605,0x0605,0x0605,0x0605,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0807,0x0807,0x0807,0x0807,0x0807
	.hword 0x0807,0x0807,0x0807,0x0807,0x0807,0x0403,0x0403,0x0403
	.hword 0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201
	.hword 0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201
	.hword 0x0403,0x0403,0x0403,0x0403,0x0403,0x0403,0x0403,0x0403
	.hword 0x0403,0x0403,0x0403,0x0403,0x0403,0x0403,0x0403,0x0403

	.section .rodata
	.align	2
	.global leveltilesPal		@ 512 unsigned chars
	.hidden leveltilesPal
leveltilesPal:
	.hword 0x14A4,0x1166,0x3DEE,0x1D87,0x1CE7,0x1DC9,0x1125,0x1586
	.hword 0x2108,0x18C6,0x21EA,0x19A6,0x1DC7,0x20E7,0x1567,0x1CC6
	.hword 0x2528,0x1988,0x1DA7,0x1DE9,0x1587,0x1987,0x19A8,0x21A9
	.hword 0x1566,0x1DC8,0x1967,0x18A5,0x1966,0x21C8,0x1DE8,0x1145
	.hword 0x1DA8,0x2A0B,0x19C7,0x1968,0x25C9,0x21C9,0x19A7,0x2529
	.hword 0x3DEF,0x1D88,0x21E9,0x25EA,0x2107,0x14A5,0x260A,0x1CE6
	.hword 0x1546,0x1986,0x2949,0x18C5,0x1545,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(leveltiles)
