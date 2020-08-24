// #LAYOUT# M65 KERNAL_1 #TAKE
// #LAYOUT# *   *        #IGNORE


m65_scrtab_vic_ctrlb:        // parameters for VIC control register B

	.byte %00100000 // 40x25 + extended attributes
	.byte %10100000 // 80x25 + extended attributes
	.byte %10100100 // 80x50 + extended attributes

m65_scrtab_colviewmax_lo:       // maximum allowed color viewport start - low byte

	.byte $10
	.byte $10
	.byte $40

m65_scrtab_colviewmax_hi:       // maximum allowed color viewport start - high byte

	.byte $27
	.byte $27
	.byte $1F

m65_scrtab_scrolx:           // values for VIC_SCROLX

	.byte $C8
	.byte $CA
	.byte $CA

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

// Jumptable for screen control codes support. To improve performance, should be sorted
// starting from the least probable routine.

m65_chrout_screen_jumptable_codes:

	.byte KEY_CLR 
	.byte KEY_HOME
	.byte KEY_C65_SHIFT_OFF
	.byte KEY_C65_SHIFT_ON
	.byte KEY_TXT
	.byte KEY_GFX
	.byte KEY_RVS_OFF
	.byte KEY_RVS_ON
	.byte KEY_CRSR_RIGHT
	.byte KEY_CRSR_LEFT
	.byte KEY_CRSR_DOWN
	.byte KEY_CRSR_UP
	.byte KEY_INS
	.byte KEY_TAB
	.byte KEY_LINE_FEED
	.byte KEY_UNDERLINE_ON
	.byte KEY_UNDERLINE_OFF
	.byte KEY_FLASHING_ON
	.byte KEY_FLASHING_OFF
	.byte KEY_TAB_SET_CLR
	.byte KEY_ESC
	.byte KEY_BELL

__m65_chrout_screen_jumptable_quote_guard:

	.byte KEY_STOP
	.byte KEY_DEL
	.byte KEY_RETURN

__m65_chrout_screen_jumptable_codes_end:


.const m65_chrout_list = List().add(

	m65_chrout_screen_CLR,
	m65_chrout_screen_HOME,
	m65_chrout_screen_SHIFT_OFF,
	m65_chrout_screen_SHIFT_ON,
	m65_chrout_screen_TXT,
	m65_chrout_screen_GFX,
	m65_chrout_screen_RVS_OFF,
	m65_chrout_screen_RVS_ON,
	m65_chrout_screen_CRSR_RIGHT,
	m65_chrout_screen_CRSR_LEFT,
	m65_chrout_screen_CRSR_DOWN,
	m65_chrout_screen_CRSR_UP,
	m65_chrout_screen_INS,
	m65_chrout_screen_TAB,
	m65_chrout_screen_LINE_FEED,
	m65_chrout_screen_UNDERLINE_ON,
	m65_chrout_screen_UNDERLINE_OFF,
	m65_chrout_screen_FLASHING_ON,
	m65_chrout_screen_FLASHING_OFF,
	m65_chrout_screen_TAB_SET_CLR,
	m65_chrout_screen_ESC,
	m65_chrout_screen_BELL,
	m65_chrout_screen_STOP,
	m65_chrout_screen_DEL,
	m65_chrout_screen_RETURN
)

m65_chrout_screen_jumptable:

	// Note: 65C02 has the page boundary vector bug fixed!
	put_jumptable(m65_chrout_list)