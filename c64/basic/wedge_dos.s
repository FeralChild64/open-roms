
// START wedge support

wedge_dos:

	// Close all the channels, so that wedge has full control
	jsr JCLALL

	// Set file parameters,  channel 15 is a typical one for commands
	jsr select_device // sets .X register
	lda #$0F
	ldy #$0F
	jsr JSETFLS

	// Now we have to check what the user wants us to do

	// Check if user asked for a status
	ldy #$00
	lda (basic_current_statement_ptr), y
	bne !+
	jmp wedge_dos_status // XXX try to optimize this, perhaps move 'wedge_dos_status' above?
!:
	// Check if asked for a directory
	cmp #$24
	bne !+
	jmp wedge_dos_directory
!:
	// Check if asked to change drive number, or if it is a regular command
	cmp #$30
	bcc wedge_dos_command // char code before 0
	cmp #$3A
	bcs wedge_dos_command // char code after 9
	iny
	lda (basic_current_statement_ptr), y
	beq wedge_dos_change_drive // end of buffer
	cmp #$20
	beq wedge_dos_change_drive
	jmp !-

wedge_dos_command:

	// Provide command name
	jsr wedge_dos_setnam
	
	jsr JSETNAM
	jsr JOPEN
	bcc !+
	jmp wedge_dos_basic_error // XXX combine with other such jumps
!:
	// Retrieve status, print it if not OK
	jsr JCLALL
	jsr wedge_dos_status_get
	lda BUF+0
	cmp #$30 // '0'
	bne wedge_dos_status_print
	lda BUF+1
	cmp #$30 // '0'
	bne wedge_dos_status_print

	// Clean-up and exit
	jmp wedge_dos_clean_exit

wedge_dos_change_drive:

	// Reuse the line number parser for device number retrieval
	jsr basic_parse_line_number
	lda basic_line_number+1
	beq !+
	jmp do_ILLEGAL_QUANTITY_error
!:
	lda basic_line_number+0
	cmp #$08
	bpl !+
	bcs !+
	jmp do_ILLEGAL_DEVICE_NUMBER_error
!:
	sta current_device_number
	jmp basic_end_of_line

wedge_dos_status_get:

	// Here the flow is mostly the same as in the example from
	// https://codebase64.org/doku.php?id=base:reading_the_error_channel_of_a_disk_drive

	// Set remaining file parameters, open the channel
	lda #$00  // empty file name
	jsr JSETNAM
	jsr JOPEN
	bcc !+
	jmp wedge_dos_basic_error // XXX combine with other such jumps
!:

	// Set channel for input
	ldx #$0F
	jsr JCHKIN
	bcc !+
	jmp wedge_dos_basic_error // XXX combine with other such jumps
!:

	ldy #$00
!:
	cpy #$50 // buffer overflow protection
	beq !+
	jsr JREADST // retrieve errors
	// XXX error in case of status != EOF
	// Print out everything retrieved from the drive
	bne !+
	jsr JCHRIN
	bcc wedge_dos_x1
	jmp wedge_dos_basic_error // XXX try to optimize this
wedge_dos_x1:
	sta BUF, y
	iny
	jmp !-
!:
	rts

wedge_dos_status:
	jsr wedge_dos_status_get
	// FALLTROUGH

wedge_dos_status_print:

	// Print buffered status
	ldx #$00
!:
	dey
	bmi !+
	lda BUF, x
	jsr JCHROUT
	inx
	bne !-
!:
	// Clean-up and exit
	jmp wedge_dos_clean_exit

wedge_dos_directory:

	// First change the secondary address to the one suitable for
	// directory loading
	lda #$00 // logical device number
	ldx current_device_number
	ldy #$60
	jsr JSETFLS

	// Provide file name
	jsr wedge_dos_setnam

	// Open the file
	jsr JOPEN
	bcs wedge_dos_basic_error

	// Set channel for file reading
	ldx #$00
	jsr JCHKIN
	bcs wedge_dos_basic_error

	// Ignore start address (2 first bytes) - XXX check for errors
	jsr JCHRIN
	bcs wedge_dos_basic_error
	jsr JCHRIN
	bcs wedge_dos_basic_error

wedge_dos_directory_line:

	// Load a single line, reuse BASIC input buffer
	ldy #$FF
!:
	iny
	jsr JCHRIN
	bcs wedge_dos_basic_error
	sta BUF, y
	cpy #$50
	beq !+ // line too long, terminate loading file
	jsr JREADST
	bne + // end of file
	cpy #$04 // 2 bytes (pointer to next line) + 2 bytes (line number) 
	bcc !-
	lda BUF, y
	bne !-
	// End of line, but not end of file
	beq wedge_dos_directory_display
!:
	lda #K_STS_END_OF_FILE
	sta IOSTATUS // make sure end of file is marked
	// FALLTROUGH

wedge_dos_directory_display:

	cpy #$05 // protection against malformed directory
	bcc wedge_dos_clean_exit

	lda #$00 // extra protection agains buffer overflow
	sta BUF, y

	// Display line
	ldx #<BUF
	stx basic_current_line_ptr+0
	ldx #>BUF
	stx basic_current_line_ptr+1
	ldx #basic_current_line_ptr
	jsr list_single_line

	// Read & display next line or quit
	lda IOSTATUS
	beq wedge_dos_directory_line
	// FALLTROUGH

wedge_dos_clean_exit:
	jsr JCLALL
	// Print new line
	lda #$0D
	jsr JCHROUT
	jmp basic_end_of_line

wedge_dos_basic_error:
	pha
	jsr JCLALL
	pla
	tax
	dex
	jmp do_basic_error

wedge_dos_setnam:
	// Now determine the length of the 'file' name
	ldy #$FF
!:
	iny
	lda (basic_current_statement_ptr), y
	bne !-
	
	// Set the name to open
	tya
	ldx basic_current_statement_ptr+0
	ldy basic_current_statement_ptr+1
	jmp JSETNAM

// END wedge support
