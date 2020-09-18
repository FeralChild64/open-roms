;; #LAYOUT# STD *       #TAKE
;; #LAYOUT# *   BASIC_0 #TAKE
;; #LAYOUT# *   *       #IGNORE

;
; Jumptable for BASIC V2 dialect functions
;

!set ITEM_B4 = fun_sgn
!set ITEM_B5 = fun_int
!set ITEM_B6 = fun_abs
!set ITEM_B7 = fun_usr
!set ITEM_B8 = fun_fre
!set ITEM_B9 = fun_pos
!set ITEM_BA = fun_sqr
!set ITEM_BB = fun_rnd
!set ITEM_BC = fun_log
!set ITEM_BD = fun_exp
!set ITEM_BE = fun_cos
!set ITEM_BF = fun_sin

!set ITEM_C0 = fun_tan
!set ITEM_C1 = fun_atn
!set ITEM_C2 = fun_peek
!set ITEM_C3 = fun_len
!set ITEM_C4 = fun_str
!set ITEM_C5 = fun_val
!set ITEM_C6 = fun_asc
!set ITEM_C7 = fun_chr
!set ITEM_C8 = fun_left
!set ITEM_C9 = fun_right
!set ITEM_CA = fun_mid


!ifndef HAS_OPCODES_65C02 {

function_jumptable_lo:

	!byte <ITEM_B4, <ITEM_B5, <ITEM_B6, <ITEM_B7
	!byte <ITEM_B8, <ITEM_B9, <ITEM_BA, <ITEM_BB, <ITEM_BC, <ITEM_BD, <ITEM_BE, <ITEM_BF
	!byte <ITEM_C0, <ITEM_C1, <ITEM_C2, <ITEM_C3, <ITEM_C4, <ITEM_C5, <ITEM_C6, <ITEM_C7
	!byte <ITEM_C8, <ITEM_C9, <ITEM_CA

function_jumptable_hi:

	!byte >ITEM_B4, >ITEM_B5, >ITEM_B6, >ITEM_B7
	!byte >ITEM_B8, >ITEM_B9, >ITEM_BA, >ITEM_BB, >ITEM_BC, >ITEM_BD, >ITEM_BE, >ITEM_BF
	!byte >ITEM_C0, >ITEM_C1, >ITEM_C2, >ITEM_C3, >ITEM_C4, >ITEM_C5, >ITEM_C6, >ITEM_C7
	!byte >ITEM_C8, >ITEM_C9, >ITEM_CA

} else { ; HAS_OPCODES_65C02

function_jumptable:

	; Note: 65C02 has the page boundary vector bug fixed!
	
	!word ITEM_B4, ITEM_B5, ITEM_B6, ITEM_B7
	!word ITEM_B8, ITEM_B9, ITEM_BA, ITEM_BB, ITEM_BC, ITEM_BD, ITEM_BE, ITEM_BF
	!word ITEM_C0, ITEM_C1, ITEM_C2, ITEM_C3, ITEM_C4, ITEM_C5, ITEM_C6, ITEM_C7
	!word ITEM_C8, ITEM_C9, ITEM_CA
}
