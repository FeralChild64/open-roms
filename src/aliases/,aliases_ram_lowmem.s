;
; Names of ZP and low memory locations:
; - Computes Mapping the Commodore 64
; - https://www.c64-wiki.com/wiki/Zeropage
; - http://unusedino.de/ec64/technical/project64/memory_maps.html
;

	; $00-$01 - 6502 CPU registers are here

	;
	; Page 0 - BASIC area ($02-$8F)
	;

	;                 $02             -- UNUSED --          free for user software
	.!set ADRAY1    = $03 ; $03-$04
	.!set ADRAY2    = $05 ; $05-$06
	.!set CHARAC    = $07 ;          [!] our implementation might be different  XXX give more details
	.!set ENDCHR    = $08 ;          [!] our implementation might be different  XXX give more details
	.!set TRMPOS    = $09 ;          -- NOT IMPLEMENTED --
	.!set VERCKB    = $0A ;          flag used by BASIC, 0 - LOAD, 1 - VERIFY
	.!set COUNT     = $0B ;          [!] our implementation might be different  XXX give more details
	.!set DIMFLG    = $0C ;          [!] our implementation might be different  XXX give more details
	.!set VALTYP    = $0D ;          expression eveluation result; $00 - float, $FF - string
	.!set INTFLG    = $0E ;          -- NOT IMPLEMENTED --
	.!set GARBFL    = $0F ;          [!] our implementation might be different  XXX give more details
	.!set SUBFLG    = $10 ;          -- NOT IMPLEMENTED --
	.!set INPFLG    = $11 ;          -- NOT IMPLEMENTED --
	.!set TANSGN    = $12 ;          -- NOT IMPLEMENTED --
	.!set CHANNL    = $13 ;          -- NOT IMPLEMENTED --
	.!set LINNUM    = $14 ; $14-$15  BASIC line number, [!] also used by DOS Wedge
	.!set TEMPPT    = $16 ;          pointer to the first available slot in the temporary string descriptor stack
	.!set LASTPT    = $17 ; $17-$18  pointer to the last used slot in the temporary string descriptor
	.!set TEMPST    = $19 ; $19-$21  temporary string stack descriptors
	.!set INDEX     = $22 ; $22-$25  temporary variables, [!] our usage might be different
	.!set RESHO     = $26 ; $26-$2A  temporary variables, [!] our usage might be different
	.!set TXTTAB    = $2B ; $2B-$2C  start of BASIC code
	.!set VARTAB    = $2D ; $2D-$2E  end of BASIC code, start of variables
	.!set ARYTAB    = $2F ; $2F-$30  pointer to start of array storage
	.!set STREND    = $31 ; $31-$32  pointer to start of free RAM
	.!set FRETOP    = $33 ; $33-$34  pointer to bottom of strings area
	.!set FRESPC    = $35 ; $35-$36  [!] our implementation uses this as temporary string pointer
	.!set MEMSIZ    = $37 ; $37-$38  highest address of BASIC memory + 1
	.!set CURLIN    = $39 ; $39-$3A  current BASIC line number
	.!set OLDLIN    = $3B ; $3B-$3C  previous BASIC line number
	.!set OLDTXT    = $3D ; $3D-$3E  current BASIC line pointer
	.!set DATLIN    = $3F ; $3F-$40  -- NOT IMPLEMENTED --
	.!set DATPTR    = $41 ; $41-$42  pointer for READ/DATA copmmands
	.!set INPPTR    = $43 ; $43-$44  -- NOT IMPLEMENTED --
	.!set VARNAM    = $45 ; $45-$46  current variable name
	.!set VARPNT    = $47 ; $47-$48  current variable/descriptor pointer
	.!set FORPNT    = $49 ; $49-$4A  -- NOT IMPLEMENTED --
	.!set OPPTR     = $4B ; $4B-$4C  helper variable for expression computation, [!] our usage details are different
	.!set OPMASK    = $4D ;          -- NOT IMPLEMENTED --
	.!set DEFPNT    = $4E ; $4E-$4F  -- NOT IMPLEMENTED --
	.!set DSCPNT    = $50 ; $50-$52  temporary area fro string handling, [!] our usage might differ
	.!set FOUR6     = $53 ;          size of variable content (float = 5, integer = 2, string descriptor = 3)
	.!set JMPER     = $54 ; $54-$56  -- NOT IMPLEMENTED --
	.!set TEMPF1    = $57 ; $57-$5B  BASIC numeric work area
	.!set TEMPF2    = $5C ; $5C-$60  BASIC numeric work area

	.!set __FAC1    = $61 ; $61-$66  floating point accumulator 1
	.!set FAC1_exponent   = $61
	.!set FAC1_mantissa   = $62 ; $62 - $65
	.!set FAC1_sign       = $66

	.!set SGNFLG    = $67 ;          -- NOT IMPLEMENTED --
	.!set BITS      = $68 ;          FAC1 overflow

	.!set __FAC2    = $69 ; $69-$6E  floating point accumulator 2 [!] also used for memory move pointers
	.!set FAC2_exponent   = $69
	.!set FAC2_mantissa   = $6A ; $6A - $6D
	.!set FAC2_sign       = $6E

	.!set ARISGN    = $6F ;          -- NOT IMPLEMENTED --
	.!set FACOV     = $70 ;          FAC1 low order mantissa
	.!set FBUFPT    = $71 ; $71-$72  -- NOT IMPLEMENTED --
	.!set CHRGET    = $73 ; $73-$8A  -- NOT IMPLEMENTED --
	.!set TXTPTR    = $7A ; $7A-$7B  current BASIC statement pointer
	.!set RNDX      = $8B ; $8B-$8F  random number seed

	;
	; Page 0 - Kernal area ($90-$FF)
	;
	
	.!set IOSTATUS  = $90
	.!set STKEY     = $91  ;          keys down clears bits. STOP - bit 7, C= - bit 6, SPACE - bit 4, CTRL - bit 2
	.!set SVXT      = $92  ;          tape reading constant [!] our tape routines use it differently
	.!set VERCKK    = $93  ;          0 = LOAD, 1 = VERIFY
	.!set C3PO      = $94  ;          flag - is BSOUR content valid
	.!set BSOUR     = $95  ;          serial bus buffered output byte
	.!set SYNO      = $96  ;          temporary tape routines storage, [!] our usage differs
	.!set XSAV      = $97  ;          temporary register storage for ASCII/tape related routines [!] usage details might differ
	.!set LDTND     = $98  ;          number of entries in LAT / FAT / SAT tables
	.!set DFLTN     = $99  ;          default input device
	.!set DFLTO     = $9A  ;          default output device
	.!set PRTY      = $9B  ;          tape storage for parity/checksum
#if !CONFIG_TAPE_NORMAL && !CONFIG_TAPE_TURBO	
	.!set DPSW      = $9C  ;          -- NOT IMPLEMENTED --
#else
    .!set COLSTORE  = $9C  ;          [!] screen border storage for tape routines 
#endif
	.!set MSGFLG    = $9D  ;          bit 6 = error messages, bit 7 = control message
	.!set PTR1      = $9E  ;          for tape support, counter of errorneous bytes
	.!set PTR2      = $9F  ;          for tape support, counter for errorneous bytes correction
	.!set TIME      = $A0  ; $A0-$A2  jiffy clock
	.!set IECPROTO  = $A3  ;          [!] IEC or pseudo-IEC protocol, >= $80 for unknown; originally named TSFCNT
	.!set TBTCNT    = $A4  ;          temporary variable for tape and IEC, [!] our usage probably differs in details
	.!set CNTDN     = $A5  ;          -- NOT IMPLEMENTED --
	.!set BUFPNT    = $A6  ;          -- NOT IMPLEMENTED --
	.!set INBIT     = $A7  ;          temporary storage for tape / RS-232 received bits
#if !CONFIG_LEGACY_SCNKEY
	.!set BITCI     = $A8  ;          -- NOT IMPLEMENTED --
	.!set RINONE    = $A9  ;          -- WIP -- (UP9600 only) RS-232 check for start bit flag
	.!set RIDDATA   = $AA  ;          -- NOT IMPLEMENTED --
	.!set RIPRTY    = $AB  ;          -- WIP -- checksum while reading tape
#endif
	.!set SAL       = $AC  ; $AC-$AD  -- XXX: describe -- (implemented screen part)
	.!set EAL       = $AE  ; $AE-$AF  -- XXX: describe -- [!] used also by screen editor, for temporary color storage when scrolling
	.!set CMP0      = $B0  ; $B0-$B1  temporary tape storage, [!] here used for BRK instruction address
	.!set TAPE1     = $B2  ; $B2-$B3  tape buffer pointer
#if !CONFIG_LEGACY_SCNKEY
	.!set BITTS     = $B4  ;          -- NOT IMPLEMENTED --
#endif
	.!set NXTBIT    = $B5  ;          -- NOT IMPLEMENTED --
#if !CONFIG_LEGACY_SCNKEY
	.!set RODATA    = $B6  ;          -- NOT IMPLEMENTED --
#endif

	.!set FNLEN     = $B7  ;          current file name length
	.!set LA        = $B8  ;          current logical_file number
	.!set SA        = $B9  ;          current secondary address
	.!set FA        = $BA  ;          current device number
	.!set FNADDR    = $BB  ; $BB-$BC  current file name pointer
	.!set ROPRTY    = $BD  ;          -- NOT IMPLEMENTED -- tape / RS-232 temporary storage
	.!set FSBLK     = $BE  ;          -- NOT IMPLEMENTED -- tape / RS-232 temporary storage
	.!set MYCH      = $BF  ;          -- NOT IMPLEMENTED --
	.!set CAS1      = $C0  ;          tape motor interlock
	.!set STAL      = $C1  ; $C1-$C2  LOAD/SAVE start address
	.!set MEMUSS    = $C3  ; $C3-$C4  temporary address for tape LOAD/SAVE
	.!set LSTX      = $C5  ;          last key matrix position [!] our usage probably differs in details
	.!set NDX       = $C6  ;          number of chars in keyboard buffer
	.!set RVS       = $C7  ;          flag, whether to print reversed characters
	.!set INDX      = $C8  ;          end of logical line (column, 0-79)
	.!set LSXP      = $C9  ; $C9-$CA  start of input, X/Y position (typo in Mapping the C64, fixed in Mapping the C128)
	.!set SFDX      = $CB  ;          -- NOT IMPLEMENTED --
	.!set BLNSW     = $CC  ;          cursor blink disable flag
	.!set BLNCT     = $CD  ;          cursor blink countdown
	.!set GDBLN     = $CE  ;          cursor saved character
	.!set BLNON     = $CF  ;          cursor visibility flag
	.!set CRSW      = $D0  ;          whether to input from screen or keyboard (if from screen, stores input offset)
	.!set PNT       = $D1  ; $D1-$D2  pointer to the current screen line
	.!set PNTR      = $D3  ;          current screen X position (logical column), 0-79
	.!set QTSW      = $D4  ;          quote mode flag
	.!set LNMX      = $D5  ;          logical line length, 39 or 79
	.!set TBLX      = $D6  ;          current screen Y position (row), 0-24
	.!set SCHAR     = $D7  ;          ASCII value of the last printed character
	.!set INSRT     = $D8  ;          insert mode flag/counter
	.!set LDTB1     = $D9  ; $D9-$F2  screen line link table, [!] our usage is different  XXX give more details
	.!set USER      = $F3  ; $F3-$F4  pointer to current color RAM location
	.!set KEYTAB    = $F5  ; $F5-$F6  pointer to keyboard lookup table
	.!set RIBUF     = $F7  ; $F7-$F8  -- WIP -- RS-232 receive buffer pointer
	.!set ROBUF     = $F9  ; $F9-$FA  -- WIP -- RS-232 send buffer pointer
	;                 $FB     $FB-$FE  -- UNUSED --          free for user software
	.!set BASZPT    = $FF  ;          -- NOT IMPLEMENTED --

	;
	; Page 1
	;

	.!set STACK     = $100  ; $100-$1FF, processor stack

	;
	; Pages 2 & 3
	;
    
	.!set BUF       = $200  ; $200-$250, BASIC line editor input buffer (81 bytes)

	; $250-$258 is the 81st - 88th characters in BASIC input, a carry over from VIC-20
	; and not used on C64 - they are used if CONFIG_LEGACY_SCNKEY is enabled.

	; [!] XXX document $251-$258 usage
	.!set LAT       = $259  ; $259-$262, logical file numbers (table, 10 bytes)
	.!set FAT       = $263  ; $263-$26C, device numbers       (table, 10 bytes)
	.!set SAT       = $26D  ; $26D-$276, secondary addresses  (table, 10 bytes)
	.!set KEYD      = $277  ; $277-$280, keyboard buffer
	.!set MEMSTR    = $281  ; $281-$282, start of BASIC memory
	.!set MEMSIZK   = $283  ; $283-$284, NOTE: Mapping the 64 erroneously has the hex as $282 (DEC is correct)
	.!set TIMOUT    = $285  ;            IEEE-488 timeout
	.!set COLOR     = $286  ;            current text foreground color
	.!set GDCOL     = $287  ;            color of character under cursor
	.!set HIBASE    = $288  ;            high byte of start of screen
	.!set XMAX      = $289  ;            max keyboard buffer size
	.!set RPTFLG    = $28A  ;            whether to repeat keys (highest bit set = repeat)
	.!set KOUNT     = $28B  ;            key repeat counter
	.!set DELAY     = $28C  ;            -- NOT IMPLEMENTED --
	.!set SHFLAG    = $28D  ;            bucky keys (SHIFT/CTRL/C=) flags
	.!set LSTSHF    = $28E  ;            last bucky key flags
	.!set KEYLOG    = $28F  ; $28F-$290  routine to setup keyboard decoding
	.!set MODE      = $291  ;            flag, is case switch allowed
	.!set AUTODN    = $292  ;            -- NOT IMPLEMENTED -- screen scroll disable
#if !CONFIG_LEGACY_SCNKEY
	.!set M51CRT    = $293  ;            -- NOT IMPLEMENTED -- mock 6551
	.!set M51CDR    = $294  ;            -- NOT IMPLEMENTED -- mock 6551
	.!set M51AJB    = $295  ; $295-$296  -- NOT IMPLEMENTED -- mock 6551
	.!set RSSTAT    = $297  ;            -- WIP -- (UP9600 only) mock 6551, RS-232 status
	.!set BITNUM    = $298  ;            -- NOT IMPLEMENTED --
	.!set BAUDOF    = $299  ; $299-$29A  -- NOT IMPLEMENTED --
#endif
	.!set RIDBE     = $29B  ;            -- WIP -- write pointer into RS-232 receive buffer
	.!set RIDBS     = $29C  ;            -- WIP -- read pointer into RS-232 receive buffer
	.!set RODBS     = $29D  ;            -- WIP -- read pointer into RS-232 send buffer
	.!set RODBE     = $29E  ;            -- WIP -- write pointer into RS-232 send buffer
	.!set IRQTMP    = $29F  ; $29F-$2A0  temporary IRQ vector storage [!] we use it for tape speed calibration instead
	.!set ENABL     = $2A1  ;            -- NOT IMPLEMENTED --
	.!set TODSNS    = $2A2  ;            -- NOT IMPLEMENTED --
	.!set TRDTMP    = $2A3  ;            -- NOT IMPLEMENTED --
	.!set TD1IRQ    = $2A4  ;            -- NOT IMPLEMENTED --
	.!set TLNIDX    = $2A5  ;            -- NOT IMPLEMENTED --
	.!set TVSFLG    = $2A6  ;            0 = NTSC, 1 = PAL
	
	; [!] $2A7-$2FF is normally free, but in our case some extra functionality uses it

#if CONFIG_MEMORY_MODEL_60K

	; IRQs are disabled when doing such accesses, and a default NMI handler only increments
	; a counter, so that if an NMI occurs, it does not crash the machine, but can be captured.

	.!set missed_nmi_flag         = $2A7
	.!set tiny_nmi_handler        = $2A8
	.!set peek_under_roms         = tiny_nmi_handler + peek_under_roms_routine - tiny_nmi_handler_routine
	.!set poke_under_roms         = tiny_nmi_handler + poke_under_roms_routine - tiny_nmi_handler_routine
	.!set memmap_allram           = tiny_nmi_handler + memmap_allram_routine   - tiny_nmi_handler_routine
	.!set memmap_normal           = tiny_nmi_handler + memmap_normal_routine   - tiny_nmi_handler_routine
#if SEGMENT_BASIC
	.!set shift_mem_up_internal   = tiny_nmi_handler + shift_mem_up_routine    - tiny_nmi_handler_routine
	.!set shift_mem_down_internal = tiny_nmi_handler + shift_mem_down_routine  - tiny_nmi_handler_routine
#endif

#endif ; CONFIG_MEMORY_MODEL_60K

	; BASIC vectors
	.!set IERROR    = $300  ; $300-$301
	.!set IMAIN     = $302  ; $302-$303
	.!set ICRNCH    = $304  ; $304-$305
	.!set IQPLOP    = $306  ; $306-$307
	.!set IGONE     = $308  ; $308-$309
	.!set IEVAL     = $30A  ; $30A-$30B
	
	.!set SAREG     = $30C  ;            .A storage, for SYS call
	.!set SXREG     = $30D  ;            .X storage, for SYS call
	.!set SYREG     = $30E  ;            .Y storage, for SYS call
	.!set SPREG     = $30F  ;            .P storage, for SYS call

	.!set USRPOK    = $310  ;            -- NOT IMPLEMENTED -- JMP instruction
	.!set USRADD    = $311  ; $311-$312  -- NOT IMPLEMENTED --
	;                 $313                -- UNUSED --          free for user software
	
	; Kernal vectors - interrupts
	.!set CINV      = $314  ; $314-$315
	.!set CBINV     = $316  ; $316-$317
	.!set NMINV     = $318  ; $318-$319
	
	; Kernal vectors - routines
	.!set IOPEN     = $31A  ; $31A-$31B
	.!set ICLOSE    = $31C  ; $31C-$31D
	.!set ICHKIN    = $31E  ; $31E-$31F
	.!set ICKOUT    = $320  ; $320-$321
	.!set ICLRCH    = $322  ; $322-$323
	.!set IBASIN    = $324  ; $324-$325
	.!set IBSOUT    = $326  ; $326-$327
	.!set ISTOP     = $328  ; $328-$329
	.!set IGETIN    = $32A  ; $32A-$32B
	.!set ICLALL    = $32C  ; $32C-$32D
	.!set USRCMD    = $32E  ; $32E-$32F
	.!set ILOAD     = $330  ; $330-$331
	.!set ISAVE     = $332  ; $332-$333

	;                 $314     $334-$33B  -- UNUSED --          free for user software
	
#if !CONFIG_RS232_UP9600
	.!set TBUFFR    = $33C  ; $33C-$3FB  [!] tape buffer, our usage details differ
#else
    .!set REVTAB    = $37C  ; $37C-$3FB  -- WIP -- [!] RS-232 precalculated data
#endif

	;                 $3FC     $3FC-$3FF  -- UNUSED --          free for user software


;
; Definitions for extended IEC functionality
;

	.const IEC_NORMAL  = 0 ; has to be 0, always!
#if CONFIG_IEC_JIFFYDOS 
	.const IEC_JIFFY   = 1 ; has to be 1, always!
#endif
#if CONFIG_IEC_DOLPHINDOS
	.const IEC_DOLPHIN = 2
#endif
#if CONFIG_IEC_BURST_CIA1 || CONFIG_IEC_BURST_CIA2 || ROM_LAYOUT_M65
	.const IEC_BURST   = 3
#endif
#if ROM_LAYOUT_M65
	.const IEC_ROMDOS  = 4
#endif


;
; Definitions for tape functionality
;

#if CONFIG_TAPE_NORMAL || CONFIG_TAPE_TURBO

	.!set __normal_time_S             = IRQTMP+0          ; duration of the short pulse
	.!set __normal_time_M             = IRQTMP+1          ; duration of the medium pulse

	.!set __turbo_half_S              = IRQTMP+0          ; half-duration of the short pulse
	.!set __turbo_half_L              = IRQTMP+1          ; half-duration of the long pulse

	.!set __pulse_threshold           = SVXT              ; pulse classification threshold
	.!set __pulse_threshold_ML        = SYNO              ; M/L pulse classification threshold, for normal only

#endif

#if CONFIG_TAPE_TURBO

	.!set __tape_turbo_bytestore      = STACK             ; location of byte storage helper routine

#endif
