// #LAYOUT# STD *       #TAKE
// #LAYOUT# *   BASIC_0 #TAKE
// #LAYOUT# *   *       #IGNORE

//
// Tries to execute the statement, and all the subsequent ones
//


execute_statements:

	// Check for RUN/STOP

	lda STKEY
	bpl_16 cmd_stop

	// Skip over any white spaces and colons (':')
!:
	jsr fetch_character
	cmp #$20                           // space, can be skipped
	beq !-
	cmp #$3A                           // colon, can be skipped
	beq !-
	
	// Check if end of the line

	cmp #$00
	beq execute_statements_end_of_line

	// Not end of the line - it seems we actually have something to execute	

	cmp #$7F
	bcc_16 do_SYNTAX_error             // branch if not a token    XXX here we should handle variable assignments

#if !HAS_OPCODES_65C02

	// Get the jump table entry for it, push it on the stack, and then RTS to start it.

	tax
	lda command_jumptable_hi - $80, x
	pha
	lda command_jumptable_lo - $80, x
	pha
	
	rts

#else // HAS_OPCODES_65C02

	// Use jumptable to go to the token

	asl
	tax
	jmp (command_jumptable, x)

#endif


execute_statements_end_of_line:

	// First check, if we are in direct mode - if so,
	// end the execution and go to main loop

	ldx CURLIN+1
	inx
	beq_16 basic_main_loop

	// Advance to the next line - first copy the line number
	// to previous line number

	lda CURLIN+0
	sta OLDLIN+0
	lda CURLIN+1
	sta OLDLIN+1
		
	// Advance the basic line pointer to the next line; if end,
	// go to the main loop

	jsr basic_follow_link_to_next_line

	lda OLDTXT+0
	ora OLDTXT+1
	beq_16 basic_main_loop
	
	// FALLTROUGH

execute_line:

	// Check if pointer is null, if so, we are at the end of the program

	ldy #0
	jsr peek_line_pointer_null_check
	bcc_16 basic_main_loop             // branch if end of program reached

	// Skip pointer and line number to get address of first statement

	lda OLDTXT+0
	clc
	adc #$04
	sta TXTPTR+0
	lda OLDTXT+1
	adc #$00
	sta TXTPTR+1

	ldy #$02

#if CONFIG_MEMORY_MODEL_60K
	ldx #<OLDTXT
	jsr peek_under_roms
#else // CONFIG_MEMORY_MODEL_38K
	lda (OLDTXT),y
#endif

	sta CURLIN+0
	iny

#if CONFIG_MEMORY_MODEL_60K
	jsr peek_under_roms
#else // CONFIG_MEMORY_MODEL_38K
	lda (OLDTXT),y
#endif

	sta CURLIN+1

	jmp execute_statements