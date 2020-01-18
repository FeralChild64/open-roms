// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE


#if CONFIG_IEC


iec_set_idle:
	lda CIA2_PRA
	ora #BIT_CIA2_PRA_CLK_OUT                                 // pull
	and #$FF - BIT_CIA2_PRA_DAT_OUT - BIT_CIA2_PRA_ATN_OUT    // release
	sta CIA2_PRA
	rts


#endif // CONFIG_IEC
