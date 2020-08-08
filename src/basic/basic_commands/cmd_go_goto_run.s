// #LAYOUT# STD *       #TAKE
// #LAYOUT# *   BASIC_0 #TAKE
// #LAYOUT# *   *       #IGNORE


cmd_go:

	jsr fetch_character_skip_spaces

	cmp #$A4                           // 'TO' keyword
	beq cmd_goto

#if !ROM_LAYOUT_M65

	// FALLTROUGH

cmd_go_syntax_error:

	jmp do_SYNTAX_error

#else 

	dew TXTPTR                         // unconsume character

	// Fetch mode

	jsr fetch_uint8

	cmp #64
	bne !+

	// If in native mode - switch to C64 compatibilty

	jsr M65_ISMODE65
	bne cmd_go_rts

	jsr helper_ask_if_sure
	bcs cmd_go_rts
	jsr M65_MODE64
	jmp do_clr
!:
	cmp #65
	bne !+

	// If in C64 compatibility mode - switch to native mode

	jsr M65_ISMODE65
	beq cmd_go_rts

	jsr helper_ask_if_sure
	bcs cmd_go_rts
	jsr M65_MODE65
	jmp do_clr
!:
	jmp do_ILLEGAL_QUANTITY_error

cmd_go_rts:

	rts

#endif

cmd_run:

	// RUN clears all variables

	jsr do_clr

	// Initialize OLDTXT pointer, disable Kernal messages

	jsr init_oldtxt
	lda #$00
	jsr JSETMSG

	// Check if we have any paramenters

	jsr is_end_of_statement
	bcs cmd_run_goto_launch

	// FALLTROUGH

cmd_goto:

	// GOTO requires line number

	jsr fetch_line_number
#if !ROM_LAYOUT_M65
	bcs cmd_go_syntax_error
#else
	bcs_16 do_SYNTAX_error
#endif

	// Check for direct mode

	lda CURLIN+1
	bmi cmd_goto_direct

	// Not a direct mode - compare desired line number (LINNUM) with the current one (CURLIN)

	lda LINNUM+1
	cmp CURLIN+1
	bne !+
	lda LINNUM+0
	cmp CURLIN+0
!:
	// If desired line number is lower - search line number from start

	bcc cmd_goto_from_start

	// Else - search from the current line

	jsr find_line_from_current
	jmp_8 cmd_goto_check

cmd_goto_direct:

	// Disable Kernal messages

	lda #$00
	jsr JSETMSG

	// FALLTROUGH

cmd_goto_from_start:

	// Direct mode - find the line from the beginning

	jsr find_line_from_start

	// FALLTROUGH

cmd_goto_check:

	bcs_16 do_UNDEFD_STATEMENT_error

	// FALLTROUGH

cmd_run_goto_launch:

	// Run it!

	pha
	pha
	jmp execute_line