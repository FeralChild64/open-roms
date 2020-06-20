// #LAYOUT# STD *       #TAKE
// #LAYOUT# M65 *       #TAKE
// #LAYOUT# X16 BASIC_0 #TAKE
// #LAYOUT# *   *       #IGNORE

//
// Tokenise a line of BASIC stored at $0200
//
// Input:
// - length in __tokenise_work1
//


tokenise_line:

#if (ROM_LAYOUT_M65 && SEGMENT_BASIC_0)

	jsr     map_BASIC_1
	jsr_ind VB1__tokenise_line
	jmp     map_NORMAL

#else

	// Reuse the CPU stack - addresses above $101 are used by 'tk_pack.s'

	.label tk__offset       = $100               // offset into string
	.label tk__length       = $101               // length of the raw string  XXX reuse __tokenise_work1?

	// Initialize variables

	lda #$00
	sta tk__offset
	sta QTSW

	ldx __tokenise_work1
	stx tk__length

	// Terminate input string with $00

	// XXX do we need this?
	sta BUF, x

	// FALLTROUGH

tokenise_line_loop:

	// Check if we have reached end of the line

	lda tk__offset
	cmp tk__length
	beq tokenise_line_done

	// Check for some special characters

	ldx tk__offset
	lda BUF, x

	cmp #$22
	beq tokenize_line_quote

	cmp #$DE
	beq tokenize_line_pi

	cmp #$3F
	beq tokenize_line_question_mark              // shortcut for PRINT command

	// Check for quote mode - in such case do not truy to tokenize 

	lda QTSW
	bne tokenize_line_char

	// Try to tokenize

	jsr tk_pack
!:
	lda tk__len_unpacked
	beq tokenize_line_char                       // branch if attempt to tokenize failed

	lda #<packed_str_keywords_V2
	sta FRESPC+0
	lda #>packed_str_keywords_V2
	sta FRESPC+1

	jsr tk_search
	bcc tokenize_line_keyword_V2                 // branch if keyword identified

	// XXX add support for additional keyword lists here

	// Shorten packed keyword candidate and try again

	jsr tk_shorten
	jmp !-                                       // XXX BRA on some CPUs

tokenise_line_done:

	// XXX

	// Update line length
	lda tk__length
	sta __tokenise_work1

	// Clear quote mode flag that is also used by KERNAL for screen display
	lda #$00
	sta QTSW

	rts

tokenize_line_keyword_V2:

	// XXX

tokenize_line_quote:

	// XXX

tokenize_line_pi:

	// XXX

tokenize_line_question_mark:

	// XXX

tokenize_line_char:

	ldx tk__offset

	// XXX

#endif // ROM layout
