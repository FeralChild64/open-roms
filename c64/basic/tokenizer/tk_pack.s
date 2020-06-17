// #LAYOUT# STD *       #TAKE
// #LAYOUT# M65 BASIC_1 #TAKE
// #LAYOUT# X16 BASIC_0 #TAKE
// #LAYOUT# *   *       #IGNORE

//
// Packs a keyword candidate using a simple compression scheme,
// same as in 'generate_strings.cc'.
//
// This allows us to search through the compressed tokens directly
// using the compressed fragment of BASIC, and thus work out the
// token for a given piece of string.
//
// This also helps ensure that we have an implementation that is totally
// different to that of the original C64 BASIC, as well as likely
// saving a bit of space in the ROM, especially once we start implementing
// larger keyword vocabs for extended BASIC, since the compression and
// decompression machinery is of fixed size.
//
// Input:
// - .X = offset of string to pack, address is BUF ($0200) + .X
// Output:
// - see labels in 'tk_pack'
//


tk_pack:

	// Output - reuse the CPU stack

	.label tk__packed       = $100 // packed candidate, max 16 bytes
	.label tk__len_packed   = $110 // length of unpacked data
	.label tk__len_unpacked = $111 // length of packed data
	.label tk__shorten_bits = $112 // for quick shortening of packed candidate

	// Helper variables

	.label tk__nibble_flag  = $113 // 0 = start from new byte, 1 = start from high nibble

	// Initialize variables

	lda #$00
	ldy #$13
!:
	sta tk__packed, y
	dey
	bpl !-

	// FALLTROUGH

tk_pack_loop:

	// Get the next character

	lda BUF, x

	// If character cannot be a part of a keyword - do not pack anything more

	cmp #$23                           // '#' - first sane character for a keyword
	bcc tk_pack_done                   // branch if below '#'
	cmp #$7B                           // 'Z' + 1
	bcs tk_pack_done                   // branch if above 'Z'

	// Keywords should not contain digits, colons, or semicolons

	cmp #$30
	bcc !+                             // branch if below '0'
	cmp #$3C
	bcc tk_pack_done                   // branch if below '<'
!:
	// Try to find a nibble to encode the character

	ldy #$0E
!:
	cmp packed_as_nibbles-1, y 
	beq tk_pack_nibble

	dey
	bne !-

	// Try to find a byte to encode the character

	// XXX

	// FALLTROUGH

tk_pack_done:

	rts

tk_pack_nibble:

	// .Y contains our nibble

	// XXX

tk_pack_byte:

	// .Y contains our nibble

	// XXX