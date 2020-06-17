// #LAYOUT# STD *       #TAKE
// #LAYOUT# M65 *       #TAKE
// #LAYOUT# X16 BASIC_0 #TAKE
// #LAYOUT# *   *       #IGNORE

//
// Tokenise a line of BASIC stored at $0200
//
// Input:
// - length in __tokenise_work1
//


tokenise_line:

#if (ROM_LAYOUT_M65 && SEGMENT_BASIC_0)

	jsr     map_BASIC_1
	jsr_ind VB1__tokenise_line
	jmp     map_NORMAL

#else

	// XXX



	rts


#endif // ROM layout
