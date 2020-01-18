// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE

//
// Kernal internal IEC routine
//
// - https://www.pagetable.com/?p=1031, https://github.com/mist64/cbmbus_doc
//


#if CONFIG_IEC


iec_cmd_close:

	jsr iec_check_channel_openclose
	bcc !+
	jmp kernalerror_FILE_NOT_INPUT
!:
	ora #$E0

	jmp common_open_close_unlsn_second


#endif // CONFIG_IEC
