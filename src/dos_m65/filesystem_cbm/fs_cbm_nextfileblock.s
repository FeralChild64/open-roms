
;
; CBM/CMD filesystem - helper routine to get the next file block
;


fs_cbm_nextfileblock:

	; Fetch the next track/sector

	lda PAR_SECTOR
	and #$01
	bne @part_2

@part_1:

	; We were reading from the 1st half of the buffer

	ldx SHARED_BUF_1+1
	lda SHARED_BUF_1+0
	bra @common_chk

@part_2:

	; We were reading from the 2nd half of the buffer

	ldx SHARED_BUF_1+$100+1
	lda SHARED_BUF_1+$100+0

@common_chk:

	bne @common_load

	lda #$00                           ; end of file
	sec
	rts

@common_load:

	sta PAR_TRACK
	stx PAR_SECTOR

	; FALLTROUGH

fs_cbm_nextfileblock_got_ts:

	; Load next track/sector

	jsr lowlevel_readsector         ; XXX handle read errors

	; Store pointer and amount of data ready to fetch

	lda #$FE
	sta FD_ACPTR_LEN+0
	lda #$00
	sta FD_ACPTR_LEN+1

	lda #$02
	sta FD_ACPTR_PTR+0
	lda #>SHARED_BUF_1
	sta FD_ACPTR_PTR+1

	lda PAR_SECTOR
	and #$01
	bne @part_2

@part_1:

	; Sector with data is located in the 1st half of the buffer

	lda SHARED_BUF_1+0
	bne @exit

	; Less than a full sector is used

	lda SHARED_BUF_1+1
	bra @not_full_buf

@part_2:

	; Sector with data is located in the 2nd half of the buffer

	inc FD_ACPTR_PTR+1

	lda SHARED_BUF_1+$100+0
	bne @exit

	; Less than a full sector is used

	lda SHARED_BUF_1+$100+0

@not_full_buf:

	sta FD_ACPTR_LEN+0

	; FALLTROUGH

@exit:

	clc
	rts
