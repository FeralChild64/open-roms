;; #LAYOUT# STD *       #TAKE-HIGH
;; #LAYOUT# *   BASIC_0 #TAKE-HIGH
;; #LAYOUT# *   *       #IGNORE

; This has to go $E000 or above - as the routines below bank out the main BASIC ROM!


#if CONFIG_MEMORY_MODEL_46K || CONFIG_MEMORY_MODEL_50K


peek_under_roms_via_TXTPTR:

	; Unmap BASIC lower ROM

	lda #$26
	sta CPU_R6510

	; Retrieve value from under ROMs

	lda (TXTPTR), y

	; FALLTROUGH

remap_BASIC_preserve_A:

	; Restore memory mapping

	pha
	lda #$27
	sta CPU_R6510
	pla

	; Quit

	rts

peek_under_roms_via_OLDTXT:

	; Unmap BASIC lower ROM

	lda #$26
	sta CPU_R6510

	; Retrieve value from under ROMs

	lda (OLDTXT), y
	jmp_8 remap_BASIC_preserve_A

peek_under_roms_via_VARPNT:

	; Unmap BASIC lower ROM

	lda #$26
	sta CPU_R6510

	; Retrieve value from under ROMs

	lda (VARPNT), y
	jmp_8 remap_BASIC_preserve_A

peek_under_roms_via_FAC1_PLUS_1:

	; Unmap BASIC lower ROM

	lda #$26
	sta CPU_R6510

	; Retrieve value from under ROMs

	lda (__FAC1+1), y
	jmp_8 remap_BASIC_preserve_A

#if ROM_LAYOUT_M65


fetch_character:

	ldy #0

	; Unmap BASIC lower ROM

	lda #$26
	sta CPU_R6510

	; Retrieve value from under ROMs, advance text pointer

	lda (TXTPTR), y
	inw TXTPTR

	; Restore memory mapping

	jmp_8 remap_BASIC_preserve_A


fetch_character_skip_spaces:

	ldy #0

	; Unmap BASIC lower ROM

	lda #$26
	sta CPU_R6510

	; Retrieve value from under ROMs, advance text pointer
!:
	lda (TXTPTR), y
	inw TXTPTR

	; Skip space characters

	cmp #$20
	beq !-

	; Restore memory mapping

	jmp_8 remap_BASIC_preserve_A


#endif

#endif
