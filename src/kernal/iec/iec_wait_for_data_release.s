// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE


#if CONFIG_IEC


iec_wait_for_data_release:

	lda CIA2_PRA
	// Check the highest bit, which is DATA IN,
	// (highest bit set = negative value)
	bpl iec_wait_for_data_release
	rts

#endif // CONFIG_IEC
