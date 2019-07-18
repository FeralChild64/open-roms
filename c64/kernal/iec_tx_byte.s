
;; Implemented based on https://www.pagetable.com/?p=1135, https://github.com/mist64/cbmbus_doc

;; Carry flag set = signal EOI

iec_tx_byte:

	;; Store byte to send on the stack
	pha

	;; Notify all devices that we are going to send a byte
	;; and it is going to be a data byte (released ATN)
	jsr iec_release_atn_clk_data

	;; Common part of iec_txbyte and iec_tx_common - waits for devices
	;; and transmits a byte

	pla
	jsr iec_tx_common
	
	;; All done - give device time to tell if they are busy by pulling DATA
	;; They should do it within 1ms
	ldx #$FF
*	lda CIA2_PRA
	;; BPL here is checking that bit 7 clears,
	;; i.e, that the DATA line is pulled by drive
	bpl +
	dex
	bne -
	bpl iec_tx_byte_error
*
	;; All done
	clc
	rts

iec_tx_byte_error:

	jsr iec_set_idle
	jmp kernalerror_DEVICE_NOT_FOUND


