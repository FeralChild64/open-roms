
;
; Provide pseudocommands (and other goodies) to make CPU optimizations easier
;

#if CONFIG_CPU_WDC_65C02 || CONFIG_CPU_CSG_65CE02 || CONFIG_CPU_CSG_4510 || CONFIG_CPU_M65_45GS02 || CONFIG_CPU_WDC_65816
	.cpu _65c02
	#define HAS_OPCODES_65C02
	#define HAS_BCD_SAFE_INTERRUPTS
#endif

#if CONFIG_CPU_CSG_65CE02 || CONFIG_CPU_CSG_4510 || CONFIG_CPU_M65_45GS02
	#define HAS_OPCODES_65CE02
#endif

#if CONFIG_CPU_CSG_4510 || CONFIG_CPU_M65_45GS02
	#define HAS_OPCODES_4510
#endif

#if CONFIG_CPU_M65_45GS02
	#define HAS_OPCODES_45GS02
#endif

#if CONFIG_CPU_WDC_65816
	#define HAS_OPCODES_65816
#endif



;
; These are not exactly for optimization - but to encourage proper usage of panic screen
;

.pseudocommand panic code
{
	; If case no panic screen is used, we can skip providing error codes within Kernal
#if CONFIG_PANIC_SCREEN
	lda code
#endif
	jmp ($E4B7)
}



;
; Macros for generating jumptables
;


.macro put_jumptable_lo(list)
{
	.for (var i = 0; i < list.size(); i++)
	{
		.byte mod((list.get(i) - 1), $100)
	}
}

.macro put_jumptable_hi(list)
{
	.for (var i = 0; i < list.size(); i++)
	{
		.byte floor((list.get(i) - 1) / $100)
	}
}

#if HAS_OPCODES_65C02

.macro put_jumptable(list)
{
	.for (var i = 0; i < list.size(); i++)
	{
		.word list.get(i)
	}
}

#endif



;
; Trick to skip a 2-byte instruction
;

.pseudocommand skip_2_bytes_trash_nvz { .byte $2C }



;
; Some additional 65CE02 instructions
;

#if HAS_OPCODES_65CE02


; Additional addressing modes

.pseudocommand jsr_ind addr
{
	.var arg = addr.getValue()
	.byte $22, mod(arg, $100), floor(arg / $100)
}

.pseudocommand jsr_ind_x addr
{
	.var arg = addr.getValue()
	.byte $23, mod(arg, $100), floor(arg / $100)
}

; Flat memory access support

.pseudocommand lda_lp arg {
	.var value = arg.getValue()
	.var type  = arg.getType()
	.if(type != AT_IZEROPAGEZ) .error "'lda_lp' only accepts AT_IZEROPAGEZ"
	.if(value > $FF )          .error "'lda_lp' requires zeropage address"
	nop
	.byte $B2, value
}

.pseudocommand sta_lp arg {
	.var value = arg.getValue()
	.var type  = arg.getType()
	.if(type != AT_IZEROPAGEZ) .error "'sta_lp' only accepts AT_IZEROPAGEZ"
	.if(value > $FF )          .error "'sta_lp' requires zeropage address"
	nop
	.byte $92, value
}

#endif



;
; Calculation shortcuts
;

.pseudocommand lda_zero                ; WARNING: do not use within interrupts
{
#if HAS_OPCODES_65CE02
	tba
#else
	lda #$00
#endif	
}



;
; Stack manipulation - some CPUs will leave .A unchanged, some will use it as temporary storage, so consider .A trashed
;

.pseudocommand phx_trash_a
{
#if HAS_OPCODES_65C02
	phx
#else
	txa
	pha
#endif	
}

.pseudocommand phy_trash_a
{
#if HAS_OPCODES_65C02
	phy
#else
	tya
	pha
#endif	
}

.pseudocommand plx_trash_a
{
#if HAS_OPCODES_65C02
	plx
#else
	pla
	tax
#endif	
}

.pseudocommand ply_trash_a
{
#if HAS_OPCODES_65C02
	ply
#else
	pla
	tay
#endif	
}



;
; Branches with far offsets, substitutes by Bxx + JMP for CPUs not supporting the instruction
;

.pseudocommand bcs_16 dst
{
#if HAS_OPCODES_65CE02
	.var offset = mod($10000 + dst.getValue() - *, $10000) -2
	.byte $B3, mod(offset, $100), floor(offset / $100)
#else
	bcc __l
	jmp dst
__l:
#endif
}

.pseudocommand bcc_16 dst
{
#if HAS_OPCODES_65CE02
	.var offset = mod($10000 + dst.getValue() - *, $10000) -2
	.byte $93, mod(offset, $100), floor(offset / $100)
#else
	bcs __l
	jmp dst
__l:
#endif
}

.pseudocommand beq_16 dst
{
#if HAS_OPCODES_65CE02
	.var offset = mod($10000 + dst.getValue() - *, $10000) -2
	.byte $F3, mod(offset, $100), floor(offset / $100)
#else
	bne __l
	jmp dst
__l:
#endif
}

.pseudocommand bne_16 dst
{
#if HAS_OPCODES_65CE02
	.var offset = mod($10000 + dst.getValue() - *, $10000) -2
	.byte $D3, mod(offset, $100), floor(offset / $100)
#else
	beq __l
	jmp dst
__l:
#endif
}

.pseudocommand bmi_16 dst
{
#if HAS_OPCODES_65CE02
	.var offset = mod($10000 + dst.getValue() - *, $10000) -2
	.byte $33, mod(offset, $100), floor(offset / $100)
#else
	bpl __l
	jmp dst
__l:
#endif
}

.pseudocommand bpl_16 dst
{
#if HAS_OPCODES_65CE02
	.var offset = mod($10000 + dst.getValue() - *, $10000) -2
	.byte $13, mod(offset, $100), floor(offset / $100)
#else
	bmi __l
	jmp dst
__l:
#endif
}

.pseudocommand bvc_16 dst
{
#if HAS_OPCODES_65CE02
	.var offset = mod($10000 + dst.getValue() - *, $10000) -2
	.byte $53, mod(offset, $100), floor(offset / $100)
#else
	bvs __l
	jmp dst
__l:
#endif
}

.pseudocommand bvs_16 dst
{
#if HAS_OPCODES_65CE02
	.var offset = mod($10000 + dst.getValue() - *, $10000) -2
	.byte $73, mod(offset, $100), floor(offset / $100)
#else
	bvc __l
	jmp dst
__l:
#endif
}


;
; Near jump, on some CPUs can be substituted with shorter instruction
;

.pseudocommand jmp_8 dst
{
#if HAS_OPCODES_65C02
	bra dst
#else
	jmp dst
#endif
}
