//
// Names of ZP and low memory locations (https://www.c64-wiki.com/wiki/Zeropage)
//

// XXX TODO - fill-in the gaps

	// $00-$01 - 6502 CPU registers are here

    //
	// Page 0 - BASIC area ($02-$8F)
    //
	
	.label TXTTAB    = $2B

    //
	// Page 0 - Kernal area ($90-$FF)
    //
	
	.label IOSTATUS  = $90
	.label STKEY     = $91  //          Keys down clears bits. STOP - bit 7, C= - bit 6, SPACE - bit 4, CTRL - bit 2
	
	.label VERCK     = $93  //          0 = LOAD, 1 = VERIVY
	
	.label C3PO      = $94  //          flag - is BSOUR content valid
	.label BSOUR     = $95  //          serial bus buffered output byte

	.label LDTND     = $98  //          number of entries in LAT / FAT / SAT tables
	.label DFLTN     = $99
	.label DFLTO     = $9A
	
	.label MSGFLG    = $9D  //          bit 6 = error messages, bit 7 = control message

	.label TIME      = $A0  // $A0-$A2, jiffy clock

	.label IEC_TMP1  = $A3  //          temporary variable for tape and IEC
	.label IEC_TMP2  = $A4  //          temporary variable for tape and IEC

	.label CMP0      = $B0  // $B0-$B1, temporary tape storage, we use it too for BRK instruction address
	
	.label FNLEN     = $B7  //          current file name length

	.label FNADDR    = $BB  // $BB-$BC, current file name pointer

	.label STAL      = $C1  // $C1-$C2, LOAD/SAVE start address
	.label MEMUS     = $C3  // $C3-$C4, temporary address for tape LOAD/SAVE

	//
	// Other low memory addresses
	//
    
	.label BUF       = $200  // $200-$250, BASIC line editor input buffer (81 bytes)

	.label LAT       = $259  // $259-$262, logical file numbers (table, 10 bytes)
	.label FAT       = $263  // $263-$26C, device numbers       (table, 10 bytes)
	.label SAT       = $26D  // $26D-$276, secondary addresses  (table, 10 bytes)

	.label MEMSTR    = $281
	.label MEMSIZ    = $283  //            NOTE: Mapping the 64 erroniously has the hex as $282, while the DEC is correct

	.label TIMOUT    = $285  //            IEEE-488 timeout

	.label HIBASE    = $288  //            high byte of start of screen

	.label RSSTAT    = $297  //            RS-232 status

	.label PALNTSC   = $2A6  //            0 = NTSC, 1 = PAL
	
	.label SAREG     = $30C  //            .A storage, for SYS call
	.label SXREG     = $30D  //            .X storage, for SYS call
	.label SYREG     = $30E  //            .Y storage, for SYS call
	.label SPREG     = $30F  //            .P storage, for SYS call

	.label USRPOK    = $310  //            JMP instruction
	.label USRADD    = $311  // $311-$312
	
	// Kernal vectors - interrupts
	.label CINV      = $314  // $314-$315
	.label CBINV     = $316  // $316-$317
	.label NMINV     = $318  // $318-$319
	
	// Kernal vectors - routines
	.label IOPEN     = $31A  // $31A-$31B
	.label ICLOSE    = $31C  // $31C-$31D
	.label ICHKIN    = $31E  // $31E-$31F
	.label ICKOUT    = $320  // $320-$321
	.label ICLRCH    = $322  // $322-$323
	.label IBASIN    = $324  // $324-$325
	.label IBSOUT    = $326  // $326-$327
	.label ISTOP     = $328  // $328-$329
	.label IGETIN    = $32A  // $32A-$32B
	.label ICLALL    = $32C  // $32C-$32D
	.label USRCMD    = $32E  // $32E-$32F
	.label ILOAD     = $330  // $330-$331
	.label ISAVE     = $332  // $332-$333

	.label TBUFFR    = $33C  // $33C-$3FB, tape buffer	
