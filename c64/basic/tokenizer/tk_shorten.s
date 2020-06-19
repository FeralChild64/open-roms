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

	ldy tk__byte_offset                // will be needed in both cases

	// Now we need to check which nibble to cut away

	bit tk__nibble_flag
	bcs tk_shorten_1n_lo

	// FALLTROUGH

tk_shorten_1n_hi:

    // Clear the high nibble only

    dey
    sta tk__byte_offset

	lda tk__packed, y
	and #$F0
	sta tk__packed, y

	// Adjust remaining counters / flags, and quit

	dec tk__nibble_flag                // $00 -> $FF

	clc
	rts

tk_shorten_1n_lo:

	// Clear the whole byte, it will be faster

	lda #$00
	sta tk__packed, y

	// Adjust remaining counters / flags, and quit

	inc tk__nibble_flag                // $FF -> $00

	clc
	rts

tk_shorten_3n:

	ldy tk__byte_offset                // will be needed in both cases

	// Now we need to check which nibble to cut away

	bit tk__nibble_flag
	bcs tk_shorten_3n_2bytes

	// FALLTROUGH

tk_shorten_3n_byte_nibble:

	// Clear a byte and high nibble

	// XXX

tk_shorten_3n_2bytes:

    // Clear the whole two bytes 


	// XXX

	clc
	rts
