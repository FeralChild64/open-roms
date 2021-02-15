
;
; Unit number handling. For detailed description see 'm65dos_bridge.s' file
;


dos_UNITCHK:

	; Before the start make sure memory content is not damaged

	jsr dos_MEMCHK
	bcc @ok
	rts
@ok:
	jsr dos_ENTER

	; Check for unit

	cmp UNIT_SDCARD
	beq dos_UNITCHK_sdcard
	cmp UNIT_FLOPPY
	beq dos_UNITCHK_floppy
	cmp UNIT_RAMDISK
	beq dos_UNITCHK_ramdisk

	; FALLTROUGH

dos_UNITCHK_fail:

	jmp dos_EXIT_SEC

dos_UNITCHK_sdcard:

	lda #$00
	+skip_2_bytes_trash_nvz

	; FALLTROUGH

dos_UNITCHK_floppy:

	lda #$01
	+skip_2_bytes_trash_nvz

	; FALLTROUGH

dos_UNITCHK_ramdisk:

	lda #$02

	jmp dos_EXIT_A
