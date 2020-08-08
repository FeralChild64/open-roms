
//
// Names of hardware registers
//
// See https://github.com/MEGA65/mega65-core/blob/master/iomap.txt
//


#if CONFIG_MB_MEGA_65

	// VIC-IV registers

	.label VIC_KEY      = $D02F
	.label VIC_CTRLA    = $D030
	.label VIC_CTRLB    = $D031

	.label VIC_XPOS     = $D050

	.label VIC_SCRNPTR  = $D060 // 4 bytes
	.label VIC_COLPTR   = $D064 // 2 bytes

	.label VIC_CHARPTR  = $D068 // 3 bytes

	// MISC registers

	.label MISC_EMU     = $D710 // to enable badlines and slow interrupts

#endif
