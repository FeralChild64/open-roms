cmd_stop:	
basic_do_break:
	jsr printf // XXX don't use printf, use packed messages!
	.byte "BREAK",0

	// Check for direct mode
	// Are we in direct mode
	lda basic_current_line_number+1
	cmp #$FF
	beq !+
	// Not direct mode
	jsr printf
	.byte " IN ",0 // XXX don't use printf, use packed messages!
	lda basic_current_line_number+1
	ldx basic_current_line_number+0
	jsr print_integer
	
!:
	lda #$0D
	jsr JCHROUT
cmd_end:
	jmp basic_main_loop
