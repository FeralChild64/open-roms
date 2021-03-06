;; #LAYOUT# STD *       #TAKE-HIGH
;; #LAYOUT# *   BASIC_0 #TAKE-HIGH
;; #LAYOUT# *   *       #IGNORE

; This has to go $E000 or above - routine below banks out the main BASIC ROM!

; Return pointer in BASIC memory space status in Z flag


!ifdef CONFIG_MEMORY_MODEL_46K_OR_50K {

is_line_pointer_null:

	; Unmap BASIC lower ROM

	lda #$26
	sta CPU_R6510

	; Check the pointer - reuse code from other routine

	jsr line_pointer_null_check

	; FALLTROUGH

remap_BASIC_preserve_P:

	; Restore memory mapping and quit

	php
	lda #$27
	sta CPU_R6510
	plp

	rts
}
