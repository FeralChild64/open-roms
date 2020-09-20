;; #LAYOUT# STD *       #TAKE
;; #LAYOUT# *   BASIC_0 #TAKE
;; #LAYOUT# *   *       #IGNORE


varstr_garbage_collect:

	; The routine utilizes the 'shift_mem_up' - which leaves INDEX+0 and INDEX+1
	; free to be used, but INDEX+2 and beyond would be overwritten

	; We will reuse TXTPTR as the moving pointer, so store it on the stack

	lda TXTPTR+0
	pha
	lda TXTPTR+1
	pha

	; Similarly, we will reuse OLDTXT as the pointer to string descriptor

	lda OLDTXT+0
	pha
	lda OLDTXT+1
	pha	

	; INDEX+0 and INDEX+1 will be our cumulative shift counter

	lda #$00
	sta INDEX+0
	sta INDEX+1

	; Start from the highest string and go downwards

	lda MEMSIZ+0
	sta TXTPTR+0
	lda MEMSIZ+1
	sta TXTPTR+1

varstr_garbage_collect_loop:

	; Check if this was the last (lowest) string (if FRETOP == TXTPTR)

	lda FRETOP+1
	cmp TXTPTR+1
	bne varstr_garbage_collect_check_bptr	
	lda FRETOP+0
	cmp TXTPTR+0
	bne varstr_garbage_collect_check_bptr	

	; End of strings - adapt FRETOP

	clc
	lda INDEX+0
	adc FRETOP+0
	sta FRETOP+0
	lda INDEX+1
	adc FRETOP+1
	sta FRETOP+1

	; FALLTROUGH

varstr_garbage_collect_end:

	; Restore OLDTXT and TXTPTR, and quit

	pla
	sta OLDTXT+1
	pla
	sta OLDTXT+0
	pla
	sta TXTPTR+1
	pla
	sta TXTPTR+0

	rts

varstr_garbage_collect_check_bptr:

	; Decrement TXTPTR by 2, so that it points to the back-pointer

!ifndef HAS_OPCODES_65CE02 {
	lda #$02
	jsr helper_TXTPTR_down_A
} else { ; HAS_OPCODES_65CE02
	dew TXTPTR
	dew TXTPTR
}

	; Copy the back-pointer to OLDTXT, check if it is NULL

!ifdef CONFIG_MEMORY_MODEL_46K_OR_50K {

	jsr helper_gc_fetch_backpointer

} else {

	ldy #$00

!ifdef CONFIG_MEMORY_MODEL_60K {
	ldx #<TXTPTR
	jsr peek_under_roms
	sta OLDTXT+0
	iny
	jsr peek_under_roms
	sta OLDTXT+1
} else { ; CONFIG_MEMORY_MODEL_38K
	lda (TXTPTR),y
	sta OLDTXT+0
	iny
	lda (TXTPTR),y
	sta OLDTXT+1
}

	ora OLDTXT+0
}

	+beq varstr_garbage_collect_unused 
	
	; The back-pointer is not NULL - string is used

	; Setup 'memmove__size' - number of bytes to copy

	ldy #$00

!ifdef CONFIG_MEMORY_MODEL_60K {
	ldx #<OLDTXT
	jsr peek_under_roms
} else ifdef CONFIG_MEMORY_MODEL_46K_50K {
	jsr peek_under_roms_via_OLDTXT
} else { ; CONFIG_MEMORY_MODEL_38K
	lda (OLDTXT),y
}

	sta memmove__size+0
	lda #$00
	sta memmove__size+1

	; The 'memmove__size' contains string length - add back-pointer size

!ifndef HAS_OPCODES_65CE02 {
	clc
	lda memmove__size+0
	adc #$02
	sta memmove__size+0
	bcc @1
	inc memmove__size+1
@1:
} else { ; HAS_OPCODES_65CE02
	inw memmove__size
	inw memmove__size
}

	; Setup 'memmove__src' - last byte of source

	iny

!ifdef CONFIG_MEMORY_MODEL_60K {
	jsr peek_under_roms
	sta memmove__src+0
	iny
	jsr peek_under_roms
	sta memmove__src+1
} else ifdef CONFIG_MEMORY_MODEL_46K_50K {
	jsr helper_gc_set_memmove_src
} else { ; CONFIG_MEMORY_MODEL_38K
	lda (OLDTXT),y
	sta memmove__src+0
	iny
	lda (OLDTXT),y
	sta memmove__src+1
}

	; The 'memmove__src' contains the first byte of source - add 'memmove__size' to it

	clc
	lda memmove__src+0
	adc memmove__size+0
	sta memmove__src+0
	lda memmove__src+1
	adc memmove__size+1
	sta memmove__src+1

	; Shift 'memmove__src' to point last byte of source

!ifndef HAS_OPCODES_65CE02 {
	sec
	lda memmove__src+0
	sbc #$01
	sta memmove__src+0
	bcs @2
	dec memmove__src+1
@2:
} else { ; HAS_OPCODES_65CE02
	dew memmove__src
}

	; Setup 'memmove__dst' - last byte of destination

	clc
	lda memmove__src+0
	adc INDEX+0
	sta memmove__dst+0
	lda memmove__src+1
	adc INDEX+1
	sta memmove__dst+1

	; Increase the string descriptor pointer by INDEX, so that it will point to the new position

	ldy #$01

!ifdef CONFIG_MEMORY_MODEL_60K {

	ldx #<OLDTXT
	jsr peek_under_roms
	clc
	adc INDEX+0
	php
	jsr poke_under_roms

	iny
	jsr peek_under_roms
	plp
	adc INDEX+1
	jsr poke_under_roms

} else ifdef CONFIG_MEMORY_MODEL_46K_50K {
	
	jsr helper_gc_increase_oldtxt

} else { ; CONFIG_MEMORY_MODEL_38K

	lda (OLDTXT),y
	clc
	adc INDEX+0
	sta (OLDTXT),y
	iny
	lda (OLDTXT),y
	adc INDEX+1
	sta (OLDTXT),y
}

	; Move string memory up

	jsr shift_mem_up

	; Update TXTPTR

	ldy #$00

!ifdef CONFIG_MEMORY_MODEL_60K {
	ldx #<OLDTXT
	jsr peek_under_roms
} else ifdef CONFIG_MEMORY_MODEL_46K_50K {
	jsr peek_under_roms_via_OLDTXT
} else { ; CONFIG_MEMORY_MODEL_38K
	lda (OLDTXT),y
}

	jsr helper_TXTPTR_down_A

	; Check if variable descriptor in __FAC1 matches the previously moved string;
	; if so - adapt it too

	lda __FAC1+2
	cmp TXTPTR+1
	bne @3
	lda __FAC1+1
	cmp TXTPTR+0
	bne @3

	clc
	adc INDEX+0
	sta __FAC1+1
	lda __FAC1+2
	adc INDEX+1
	sta __FAC1+2
@3:
	; Same for __FAC2

	lda __FAC2+2
	cmp TXTPTR+1
	bne @4
	lda __FAC2+1
	cmp TXTPTR+0
	bne @4

	clc
	adc INDEX+0
	sta __FAC2+1
	lda __FAC2+2
	adc INDEX+1
	sta __FAC2+2
@4:
	; Next iteration

	jmp varstr_garbage_collect_loop

varstr_garbage_collect_unused:

	; The back-pointer is NULL - string is a garbage to collect
	; Fetch the string length

!ifndef HAS_OPCODES_65CE02 {
	lda #$01
	jsr helper_TXTPTR_down_A
} else { ; HAS_OPCODES_65CE02
	dew TXTPTR
}

	ldy #$00

!ifdef CONFIG_MEMORY_MODEL_60K {
	ldx #<TXTPTR
	jsr peek_under_roms
} else ifdef CONFIG_MEMORY_MODEL_46K_50K {
	jsr peek_under_roms_via_TXTPTR
} else { ; CONFIG_MEMORY_MODEL_38K
	lda (TXTPTR),y
}

	tax

	; Increase INDEX (shift offset) by string length + back-pointer length

	lda #$03
	jsr helper_INDEX_up_A
	txa
	jsr helper_INDEX_up_A

	; Decrease TXTPTR by unused string length-1

	txa
	jsr helper_TXTPTR_down_A

	; Next iteration

	jmp varstr_garbage_collect_loop
