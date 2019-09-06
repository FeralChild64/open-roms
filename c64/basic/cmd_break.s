cmd_stop:	
basic_do_break:
	jsr printf // XXX don't use printf, use packed messages!
	.text "BREAK"
	.byte 0

	// Check for direct mode
	// Are we in direct mode
	lda basic_current_line_number+1
	cmp #$FF
	beq !+
	// Not direct mode
	jsr printf // XXX don't use printf, use packed messages!
	.text " IN "
	.byte 0
	lda basic_current_line_number+1
	ldx basic_current_line_number+0
	jsr print_integer
	
!:
	lda #$0D
	jsr JCHROUT
cmd_end:
	jmp basic_main_loop
