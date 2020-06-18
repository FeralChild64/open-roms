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

	.label tk__packed       = $100 // packed candidate, 13 bytes is enough for worst case 8 byte keyword
	.label tk__full_packed  = $10D // length of packed data (full bytes! half-used does not count!)
	.label tk__len_unpacked = $10E // length of unpacked data
	.label tk__shorten_bits = $10F // for quick shortening of packed candidate

	// Helper variables

	.label tk__nibble_flag  = $110 // $00 = start from new byte, $FF = start from high nibble

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

	ldy #tk__packed_as_bytes
!:
	cmp packed_as_bytes-1, y 
	beq tk_pack_byte

	dey
	bne !-

	// If it is not on the list, it cannot be a part of token - quit

	// FALLTROUGH

tk_pack_done:

	rts

tk_pack_nibble:

	// .Y contains our nibble; check whether we should start a new encoded byte

	bit tk__nibble_flag
	bcc tk_pack_nibble_new_byte

	// Store in the high nibble of the current byte

	tya

	asl
	asl
	asl
	asl

	ldy tk__full_packed
	ora tk__packed, y
	sta tk__packed, y

	// Adjust remaining counters / flags

	inc tk__full_packed
	inc tk__nibble_flag                // $FF -> $00

	// FALLTROUGH

tk_pack_nibble_done:

	clc
	ror tk__shorten_bits

	// FALLTROUGH

tk_pack_loop_next:

	// Prepare for next loop iteration - increment counters

	inx
	inc tk__len_unpacked

	// Check length, if it is OK to pack one more byte; quit if not

	lda tk__len_unpacked
	cmp #$07                           // for now max keyword length is 7 bytes, this encoder supports up to 8

	bne tk_pack_loop
	rts

tk_pack_nibble_new_byte:

	// XXX

	// Adjust remaining counters / flags

	dec tk__nibble_flag                // $00 -> $FF
	bmi tk_pack_nibble_done

tk_pack_byte:

	// .Y contains our byte; check whether we should start a new encoded byte

	bit tk__nibble_flag
	bcc tk_pack_byte_new_byte

	// XXX

	// FALLTROUGH

tk_pack_byte_done:

	sec
	ror tk__shorten_bits

	bcc tk_pack_loop_next              // branch always

tk_pack_byte_new_byte:

	// XXX
