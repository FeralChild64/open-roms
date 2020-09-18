;; #LAYOUT# STD *       #TAKE
;; #LAYOUT# *   BASIC_0 #TAKE
;; #LAYOUT# *   *       #IGNORE


fetch_variable_arr:

	jsr find_array
	bcc @1

	; Array does not exist - we will have to create one with default parameters

	; XXX implement this

	jmp do_NOT_IMPLEMENTED_error
@1:
	; Fetch the 'coordinates'

	lda FOUR6
	pha                                          ; FOUR6 might get overridden, but we'll need it

	ldx #$00                                     ; counts number of dimensions

	; FALLTROUGH

fetch_variable_arr_fetch_coords_loop:

	inx

	; Fetch the coordinate and put it on the stack

	jsr helper_fetch_arr_coord

	lda LINNUM+1
	pha
	lda LINNUM+0
	pha

	; If more coordinates given - next iteration

	cpy #$00
	beq fetch_variable_arr_fetch_coords_loop

	; Store number of dimensions, will be needed later

	stx __FAC1+0

	; FALLTROUGH

fetch_variable_arr_check_dimensions:

	; Check if the number of dimensions matches the stored ones

!ifdef CONFIG_MEMORY_MODEL_60K {
	
	phx_trash_a
	ldx #<VARPNT
	ldy #$04
	jsr peek_under_roms
	sta INDEX+5
	plx_trash_a
	cpx INDEX+5

} else ifdef CONFIG_MEMORY_MODEL_46K_OR_50K {

	jsr helper_array_check_no_dims

} else { ; CONFIG_MEMORY_MODEL_38

	ldy #$04
	txa
	cmp (VARPNT), y
}

	bne fetch_variable_arr_bad_subscript

	; FALLTROUGH

fetch_variable_arr_calc_pos:

	; Move VARPNT po point to sizes in each dimension

	lda #$05
	jsr helper_VARPNT_up_A

	; Set the initial offset as '0'

	lda #$00
	sta __FAC1+1
	sta __FAC1+2

	ldx #$00

	; FALLTROUGH

fetch_variable_arr_calc_pos_loop:

	; Fetch the current dimension

	txa
	inx
	asl
	tay

!ifdef CONFIG_MEMORY_MODEL_60K {
	
	ldx #<VARPNT

	jsr peek_under_roms
	sta __FAC1+4
	iny
	jsr peek_under_roms
	sta __FAC1+3
	iny

} else ifdef CONFIG_MEMORY_MODEL_46K_OR_50K {

	jsr helper_array_fetch_dimension

} else { ; CONFIG_MEMORY_MODEL_38

	lda (VARPNT), y
	sta __FAC1+4
	iny
	lda (VARPNT), y
	sta __FAC1+3
	iny
}

	; Multiply offset by current dimension

	jsr helper_array_create_mul

	; Fetch the coordinate from stack; also add it to offset

	clc
	pla
	sta INDEX+0
	adc __FAC1+1
	sta __FAC1+1
	pla
	sta INDEX+1
	adc __FAC1+2
	sta __FAC1+2

	; Compare current coordinate (INDEX+0/+1) with current dimension size (__FAC1+3/+4)

	lda INDEX+1
	cmp __FAC1+4
	bcc @2
	bne fetch_variable_arr_bad_subscript

	lda INDEX+0
	cmp __FAC1+3
	bcs fetch_variable_arr_bad_subscript
@2:
	; Check if there are more dimensions to handle

	cpx __FAC1+0
	bne fetch_variable_arr_calc_pos_loop

	; FALLTROUGH

fetch_variable_arr_calc_pos_loop_done:

	; Retrieve FOUR6, use it to calculate final offset

	pla

	sta FOUR6                          ; XXX is it needed?
	sta __FAC1+3
	lda #$00
	sta __FAC1+4                       ; .X is 0 at this moment

	jsr helper_array_create_mul

	; Increment VARPNT by number of dimensions * 2, to point start of data

	lda __FAC1+0
	asl
	jsr helper_VARPNT_up_A

	; Increment VARPNT by the calculated offset and quit

	clc
	lda __FAC1+1
	adc VARPNT+0
	sta VARPNT+0
	lda __FAC1+2
	adc VARPNT+1
	sta VARPNT+1

	rts

fetch_variable_arr_bad_subscript:

	jmp do_BAD_SUBSCRIPT_error
