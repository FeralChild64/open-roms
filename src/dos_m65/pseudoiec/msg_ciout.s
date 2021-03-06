
;
; Implementation of ACPTR command. For detailed description see 'm65dos_bridge.s' file.
;


msg_CIOUT:

	jsr dos_ENTER
	php

	; Check operation mode

	ldx IDX1_LISTENER
	bmi msg_CIOUT_fail
	lda XX_MODE, x

	cmp #$01
	bne @1

	; Receiving command or file name

	ldx IDX2_LISTENER
	lda REG_A
	jmp (msg_CIOUT_cmdfn_vectab, X)
@1:
	; XXX handle cases of transferring data

	plp
	jmp dos_EXIT_SEC

msg_CIOUT_fail:

	; XXX should we set status?
	lda #K_ERR_DEVICE_NOT_FOUND

	jmp dos_EXIT_SEC_A



; Receiving command/file name

msg_CIOUT_cmdfn_SD:                    ; get next byte of status - SD card

	ldx SD_CMDFN_IDX
	sta SD_CMDFN_BUF,x
	inx
	bpl @1
	ldx #$7F
@1:
	stx SD_CMDFN_IDX
	lda #$A0
	sta SD_CMDFN_BUF,x
	plp
	+bcs unit_sd_cmd_OPEN_EOI          ; if EOI - execute command
	jmp dos_EXIT_CLC

msg_CIOUT_cmdfn_FD:                    ; get next byte of status - floppy

	ldx FD_CMDFN_IDX
	sta FD_CMDFN_BUF,x
	inx
	bpl @1
	ldx #$7F
@1:
	stx FD_CMDFN_IDX
	lda #$A0
	sta FD_CMDFN_BUF,x
	plp
	+bcs unit_fd_cmd_OPEN_EOI          ; if EOI - execute command
	jmp dos_EXIT_CLC

msg_CIOUT_cmdfn_RD:                    ; get next byte of status - ram disk

	ldx RD_CMDFN_IDX
	sta RD_CMDFN_BUF,x
	inx
	bpl @1
	ldx #$7F
@1:
	stx RD_CMDFN_IDX
	lda #$A0
	sta RD_CMDFN_BUF,x
	plp
	+bcs unit_rd_cmd_OPEN_EOI          ; if EOI - execute command
	jmp dos_EXIT_CLC

msg_CIOUT_cmdfn_vectab:

	!word msg_CIOUT_cmdfn_SD
	!word msg_CIOUT_cmdfn_FD
	!word msg_CIOUT_cmdfn_RD
