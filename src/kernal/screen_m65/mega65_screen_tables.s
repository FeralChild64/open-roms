// #LAYOUT# M65 KERNAL_1 #TAKE
// #LAYOUT# *   *        #IGNORE


m65_scrtab_vic_ctrlb:        // parameters for VIC control register B

	.byte %00100000 // 40x25 + extended attributes
	.byte %10100000 // 80x25 + extended attributes
	.byte %10100100 // 80x50 + extended attributes

m65_scrtab_txtwidth:         // width of the text screen (logical width is always set to 80)

	.byte 40
	.byte 80
	.byte 80

m65_scrtab_txtheight:        // height of the text screen

	.byte 25
	.byte 25
	.byte 50

m65_scrtab_viewmax_lo:       // maximum allowed viewport start - low byte

	.byte $B0
	.byte $B0
	.byte $E0

m65_scrtab_viewmax_hi:       // maximum allowed viewport start - high byte

	.byte $96
	.byte $96
	.byte $8E

m65_scrtab_rowoffset_lo:     // row offsets - low bytes

	.byte <(80 *  0), <(80 *  1), <(80 *  2), <(80 *  3), <(80 *  4), <(80 *  5), <(80 *  6), <(80 *  7), <(80 *  8), <(80 *  9)
	.byte <(80 * 10), <(80 * 11), <(80 * 12), <(80 * 13), <(80 * 14), <(80 * 15), <(80 * 16), <(80 * 17), <(80 * 18), <(80 * 19)
	.byte <(80 * 20), <(80 * 21), <(80 * 22), <(80 * 23), <(80 * 24), <(80 * 25), <(80 * 26), <(80 * 27), <(80 * 28), <(80 * 29)
	.byte <(80 * 30), <(80 * 31), <(80 * 32), <(80 * 33), <(80 * 34), <(80 * 35), <(80 * 36), <(80 * 37), <(80 * 38), <(80 * 39)
	.byte <(80 * 40), <(80 * 41), <(80 * 42), <(80 * 43), <(80 * 44), <(80 * 45), <(80 * 46), <(80 * 47), <(80 * 48), <(80 * 49)

m65_scrtab_rowoffset_hi:     // row offsets - high bytes

	.byte >(80 *  0), >(80 *  1), >(80 *  2), >(80 *  3), >(80 *  4), >(80 *  5), >(80 *  6), >(80 *  7), >(80 *  8), >(80 *  9)
	.byte >(80 * 10), >(80 * 11), >(80 * 12), >(80 * 13), >(80 * 14), >(80 * 15), >(80 * 16), >(80 * 17), >(80 * 18), >(80 * 19)
	.byte >(80 * 20), >(80 * 21), >(80 * 22), >(80 * 23), >(80 * 24), >(80 * 25), >(80 * 26), >(80 * 27), >(80 * 28), >(80 * 29)
	.byte >(80 * 30), >(80 * 31), >(80 * 32), >(80 * 33), >(80 * 34), >(80 * 35), >(80 * 36), >(80 * 37), >(80 * 38), >(80 * 39)
	.byte >(80 * 40), >(80 * 41), >(80 * 42), >(80 * 43), >(80 * 44), >(80 * 45), >(80 * 46), >(80 * 47), >(80 * 48), >(80 * 49)