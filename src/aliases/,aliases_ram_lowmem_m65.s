;; #LAYOUT# M65 * #TAKE
;; #LAYOUT# *   * #IGNORE

;
; Names of ZP and low memory locations for the native MEGA65 mode
;


	;
	; Page 0 - reused LDTB1 and USER
	;

	.!set M65_TMP__K1   = $D9  ; $D9      -- UNUSED -- temporary value for various Kernal routines
	.!set M65_LPNT_BAS1 = $DA  ; $DA-$DD  4-byte pointer for temporary usage within BASIC
	.!set M65_LPNT_BAS2 = $DE  ; $DE-$E1  4-byte pointer for temporary usage within BASIC
	.!set M65_LPNT_SCR  = $E2  ; $E2-$E5  4-byte pointer for temporary usage within BASIC and screen editor
	                            ; E6      reserved for BASIC error handling routines, also in native mode
	.!set M65_MAGICSTR  = $E7  ; $E7-$E9  if equals 'M65' it means we are in native mode
	                            ; EA      reserved for BASIC error handling routines, also in native mode
	.!set M65_LPNT_IRQ  = $EB  ; $EB-$EE  4-byte pointer for temporary usage within interrupts
	.!set M65_LPNT_KERN = $EF  ; $EF-$F3  4-byte pointer for temporary usage by KERNAL
	.!set M65_TMP__K2   = $F4  ; $F4      -- UNUSED -- temporary value for various Kernal routines

	;
	; Page 0 - other reused values
	;

	.!set M65__TXTROW   = TBLX  ;         screen row
	.!set M65__TXTCOL   = PNTR  ;         screen column
	.!set M65__SCRINPUT = LSXP  ;         pointer to the screen input, within the screen segment

	;
	; Pages 4-7 - reused screen memory
	;

	; $400-$57F - reserved for BASIC

	                               ; $400-$57F  -- UNUSED -- reserved for BASIC

	; $580-$5FF - reserved for KERNAL

	.!set M65_SCRSEG       = $580 ; $580-$581  segment address (2 higher bytes) of the screen memory
	.!set M65_SCRBASE      = $582 ; $582-$583  first byte of screen memory
	.!set M65_SCRGUARD     = $584 ; $584-$585  last byte of screen memory + 1  XXX is it needed?
	.!set M65_COLGUARD     = $586 ; $586-$587  last byte of colour memory + 1
	.!set M65_COLVIEW      = $588 ; $588-$589  first byte of color memory viewport
	.!set M65_COLVIEWMAX   = $58A ; $58A-$58B  largest allowed value of M65_COLVIEW
	.!set M65_SCRMODE      = $58C ;            0 = 40x25, 1 = 80x25, 2 = 80x50 
	.!set M65_SCRWINMODE   = $58D ;            $00 = normal mode, $FF = window enabled
	.!set M65_TXTWIN_X0    = $58E ;            text window - top-left X coordinate, starting from 0
	.!set M65_TXTWIN_Y0    = $58F ;            text window - top-left Y coordinate, starting from 0
	.!set M65_TXTWIN_X1    = $590 ;            text window - bottom-right X coordinate + 1
	.!set M65_TXTWIN_Y1    = $591 ;            text window - bottom-right Y coordinate + 1
	.!set M65_TXTROW_OFF   = $592 ; $592-$593  offset to the current text row from the viewport start
	.!set M65_SCRCOLMAX    = $594 ;            maximum allowed column
	                               ; $595-$5EB  -- UNUSED --
	.!set M65_DMAGIC_LIST  = $5EF ; $5EF-$5FF  reserved for DMAgic list, 17 bytes
	.!set M65_RS232_INBUF  = $600 ; $600-$6FF  -- reserved for RS-232 input buffer --
	.!set M65_RS232_OUTBUF = $700 ; $700-$7FF  -- reserved for RS-232 output buffer --

	;
	; Addresses for configuring the DMA job
	;

	.!set M65_DMAJOB_SIZE_0    = M65_DMAGIC_LIST + 7
	.!set M65_DMAJOB_SIZE_1    = M65_DMAGIC_LIST + 8

	.!set M65_DMAJOB_SRC_0     = M65_DMAGIC_LIST + 9
	.!set M65_DMAJOB_SRC_1     = M65_DMAGIC_LIST + 10
	.!set M65_DMAJOB_SRC_2     = M65_DMAGIC_LIST + 11
	.!set M65_DMAJOB_SRC_3     = M65_DMAGIC_LIST + 2

	.!set M65_DMAJOB_DST_0     = M65_DMAGIC_LIST + 12
	.!set M65_DMAJOB_DST_1     = M65_DMAGIC_LIST + 13
	.!set M65_DMAJOB_DST_2     = M65_DMAGIC_LIST + 14
	.!set M65_DMAJOB_DST_3     = M65_DMAGIC_LIST + 4
