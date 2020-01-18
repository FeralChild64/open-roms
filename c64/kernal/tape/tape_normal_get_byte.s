// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE

//
// Tape (normal) helper routine - byte reading
//
// Returns byte in .A (also sotred in INBIT), Carry flag set = error
//


#if CONFIG_TAPE_NORMAL


tape_normal_get_byte:

	// Init parity bit and canary bit
	lda #$01
	sta PRTY
	lda #%01111111                     // canary bit is 0
	sta INBIT

	// Now fetch individual bits

tape_normal_get_byte_loop:

	jsr tape_normal_get_bit
	bcs tape_normal_get_byte_error
	beq !+

	// Handle parity bit
	lda #$01
	eor PRTY
	sta PRTY

	sec
!:                                     // moved bit state from Zero to Carry flag

	ror INBIT                          // put the bit in
	bcs tape_normal_get_byte_loop      // loop if no canary bit reached

	// Byte retrieved (on stack), now we need to validate the parity

	jsr tape_normal_get_bit
	bcs tape_normal_get_byte_error
	beq !+
	lda #$01
!:
	eor PRTY
	bne tape_normal_get_byte_error

	// Checksum validated succesfully
	
	ldx #$0B
	// Carry already clear

	// FALLTROUGH

tape_normal_get_byte_done:

	stx VIC_EXTCOL
	lda INBIT
	rts


tape_normal_get_byte_error:

	ldx #$09
	sec                                // make sure Carry is set to indicate error
	bcs tape_normal_get_byte_done


#endif
