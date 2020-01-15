// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE

//
// Clear the SID settings - for sound effects during LOAD
//


#if CONFIG_TAPE_TURBO


tape_clean_sid:

	lda #$00
	ldy #$1C
!:
	sta __SID_BASE, y
	dey
	bpl !-

	rts


#endif
