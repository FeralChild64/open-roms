// #LAYOUT# STD *       #TAKE
// #LAYOUT# M65 BASIC_1 #TAKE
// #LAYOUT# X16 BASIC_0 #TAKE
// #LAYOUT# *   *       #IGNORE

//
// Shortens a keyword candidate previously packed by tk_pack by 1 character
//
// Input:
// - uses same variables as 'tk_pack'
// Output:
// - sets Carry if no longer possible to shorten keyword candidate
//


tk_shorten:

	// First make sure we actually can shorten anything

	dec tk__len_unpacked
	bne !+

	sec
	rts
!:
	// Now check if the last character was encoded as 1 nibble or 3 nibbles

	asl tk__nibble_flag
	bcs tk_shorten_3n

	// FALLTROUGH

tk_shorten_1n:

	// XXX

	clc
	rts

tk_shorten_3n:

	// XXX

	clc
	rts
