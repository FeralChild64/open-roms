// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE

//
// Official Kernal routine, described in:
//
// - [RG64] C64 Programmers Reference Guide   - page 304
// - [CM64] Computes Mapping the Commodore 64 - page 224
// - https://www.pagetable.com/?p=1031, , https://github.com/mist64/cbmbus_doc
// - http://www.zimmers.net/anonftp/pub/cbm/programming/serial-bus.pdf
//
// CPU registers that has to be preserved (see [RG64]): .X, .Y
//


UNTLK:

#if CONFIG_IEC

	// According to serial-bus.pdf (page 15) this routine flushes the IEC out buffer
	jsr iec_tx_flush

#if CONFIG_IEC_JIFFYDOS || CONFIG_IEC_DOLPHINDOS

	lda #$FF                           // do not use JiffyDOS / DolphinDOS anymore, trigger JiffyDOS detection
	sta IECPROTO

#endif

	// Buffer empty, send the command
	lda #$5F

common_untlk_tksa: // common part of UNTLK and TKSA

	sta TBTCNT
	jsr iec_tx_command
	bcs !+ // branch if error
	// XXX - for TKSA, is it really the right place to do a turnaround? I have some doubts
	// (it forces us to send the TKSA command 'manually' sometimes), but
	// Luigi Di Fraia (https://luigidifraia.wordpress.com/2017/06/27/codebase64-on-the-kernal-behaviour/)
	// claims TKSA Kernal routine actually does the turnaround
	jmp iec_turnaround_to_listen
!:
	rts

#else

	jmp kernalerror_ILLEGAL_DEVICE_NUMBER

#endif
