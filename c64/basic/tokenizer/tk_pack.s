// #LAYOUT# STD *       #TAKE
// #LAYOUT# M65 BASIC_1 #TAKE
// #LAYOUT# X16 BASIC_0 #TAKE
// #LAYOUT# *   *       #IGNORE


//
// Packs a keyword candidate for comparing against the list of packed keywords
//

tk_pack:

	// Variables used - reuses CPU stack

	.label tk__packed       = $100 // packed candidate, max 16 bytes
	.label tk__len_packed   = $110 // length of unpacked data
	.label tk__len_unpacked = $111 // length of packed data
	.label tk__shorten_bits = $112 // for quick shortening of packed candidate

	// XXX

	rts
