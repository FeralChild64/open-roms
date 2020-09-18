;; #LAYOUT# STD *       #TAKE
;; #LAYOUT# *   BASIC_0 #TAKE
;; #LAYOUT# *   *       #IGNORE

;
; Fetches a single character, but skips spaces
;


!set NEEDED = 1
; For these configurations we have optimized version in another file
!ifdef CONFIG_MB_M65                  { !set NEEDED = 0 }
!ifdef CONFIG_MEMORY_MODEL_46K_OR_50K { !set NEEDED = 0 }

!if NEEDED {

fetch_character_skip_spaces:

	jsr fetch_character
	cmp #$20
	beq fetch_character_skip_spaces

	rts
}
