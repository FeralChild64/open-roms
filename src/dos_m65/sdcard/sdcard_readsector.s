
;
; Reads the sector from the SD card
;

; Based on https://github.com/MEGA65/mega65-libc/blob/master/cc65/src/sdcard.c


; XXX provide documentation


sdcard_readsector:

	; XXX check if card initialized and operable

	; Set read address

	jsr sdcard_set_addr
	bcs sdcard_readsector_fail 

	; FALLTROUGH

sdcard_readsector_read:

	; Set number of retries

	lda #$0A
	sta CARD_TMP_RETRIES

	; FALLTROUGH

sdcard_readsector_try_loop:

	; Wait till card is ready

	jsr sdcard_readsector_wait
	bcs sdcard_readsector_fail

	; Read command

	lda #$02
	sta SD_CTL

	; Wait for command completion

	jsr sdcard_readsector_wait
	bcs sdcard_readsector_fail


	lda SD_CTL
	and #$67
	bne sdcard_readsector_copy_data

	jsr sdcard_reset
	dec CARD_TMP_RETRIES
	bne sdcard_readsector_try_loop

	; FALLTROUGH

sdcard_readsector_fail:

	sec
	rts


;
; Helper routines
;

sdcard_readsector_copy_data:

	; XXX use DMAgic for this

	phz
	phx

	ldx #$00
	ldz #$00

@1:
	lda [CARD_BUFPNT],z
	sta CARD_BUF,x
	inc CARD_BUFPNT+1
	lda [CARD_BUFPNT],z
	sta CARD_BUF+$100,x
	dec CARD_BUFPNT+1	

	inz
	inx
	bne @1

	; Restore registers and quit

	plx
	plz

	clc
	rts

sdcard_readsector_wait:

	; Wait till card is ready

	lda SD_CTL
	; Sometimes we see this result, i.e., sdcard.vhdl thinks it is done,
	; but sdcardio.vhdl thinks not. This means a read error.
	cmp #$01
	beq sdcard_readsector_wait_fail
	tax
	and #$40
	bne sdcard_readsector_wait_fail
	txa
	and #$03
	bne sdcard_readsector_wait

	clc
	rts

sdcard_readsector_wait_fail:

	sec
	rts
