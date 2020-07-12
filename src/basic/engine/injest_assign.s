// #LAYOUT# STD *       #TAKE
// #LAYOUT# *   BASIC_0 #TAKE
// #LAYOUT# *   *       #IGNORE

//
// Consummes assign operator in a BASIC code, Carry set if not found. Injests all spaces.
//


injest_assign:

	jsr fetch_character_skip_spaces
	cmp #$B2                           // assign operator character
	beq injest_assign_found
	
	// Not found
	jsr unconsume_character

	sec
	rts

injest_assign_found:

	clc
	rts
