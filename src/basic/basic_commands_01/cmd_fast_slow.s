;; #LAYOUT# STD *       #TAKE
;; #LAYOUT# *   BASIC_0 #TAKE
;; #LAYOUT# *   *       #IGNORE


#if CONFIG_MB_MEGA_65

	; For MEGA65 Kernal routines are called directly

#elif CONFIG_MB_ULTIMATE_64

	; Command implementation for Ultimate 64 - SuperCPU compatible way should be enough

cmd_slow:

	sta SCPU_SPEED_NORMAL
	rts

cmd_fast:

	sta SCPU_SPEED_TURBO
	rts

#elif CONFIG_PLATFORM_COMMODORE_64

	; Command implementation for generic C64 platform

cmd_slow:

	; Try to disable turbo in SuperCPU compatible way
	sta SCPU_SPEED_NORMAL

	; Try to disable turbo mode in C128 compatible way

	lda #$00
	beq !+                                       ; branch always

cmd_fast:

	; Try to enable turbo in SuperCPU compatible way
	sta SCPU_SPEED_TURBO

	; Try to enable turbo mode in C128 compatible way
	lda #$01
!:
	sta VIC_CLKRATE
	rts

#else

cmd_slow:

	nop                                          ; just to prevent double label

	; FALLTROUGH

cmd_fast:

	jmp do_NOT_IMPLEMENTED_error

#endif
