;; #LAYOUT# STD *        #TAKE
;; #LAYOUT# *   KERNAL_0 #TAKE
;; #LAYOUT# *   *        #IGNORE

;
; Messages to be prined out by Kernal
;


; Double underscore prevents labels from being passed to VICE (would confuse monitor)

!addr __MSG_KERNAL_SEARCHING_FOR      = __msg_kernalsearching_for      - __msg_kernal_first
!addr __MSG_KERNAL_LOADING            = __msg_kernalloading            - __msg_kernal_first
!addr __MSG_KERNAL_VERIFYING          = __msg_kernalverifying          - __msg_kernal_first
!addr __MSG_KERNAL_SAVING             = __msg_kernalsaving             - __msg_kernal_first
!addr __MSG_KERNAL_FROM_HEX           = __msg_kernalfrom_hex           - __msg_kernal_first
!addr __MSG_KERNAL_TO_HEX             = __msg_kernalto_hex             - __msg_kernal_first

!ifdef HAS_TAPE {
!addr __MSG_KERNAL_PRESS_PLAY         = __msg_kernalplay               - __msg_kernal_first
!addr __MSG_KERNAL_FOUND              = __msg_kernalfound              - __msg_kernal_first
!ifdef CONFIG_MB_M65 {
!addr __MSG_KERNAL_OK_SEARCHING       = __msg_kernaloksearching        - __msg_kernal_first
} }

!ifdef CONFIG_PANIC_SCREEN {
!addr __MSG_KERNAL_PANIC              = __msg_kernalpanic              - __msg_kernal_first
!addr __MSG_KERNAL_PANIC_ROM_MISMATCH = __msg_kernalpanic_rom_mismatch - __msg_kernal_first
}


__msg_kernal_first:

__msg_kernalsearching_for:
	!byte $0D
	!pet  "SEARCHING FOR"
	!byte $80 + $20 ; end of string mark + space

__msg_kernalloading:
	!byte $0D
	!pet  "LOADIN"
	!byte $80 + $47 ; end of string mark + 'G'

__msg_kernalverifying:
	!byte $0D
	!pet  "VERIFYIN"
	!byte $80 + $47 ; end of string mark + 'G'

__msg_kernalsaving:
	!byte $0D
	!pet  "SAVING"
	!byte $80 + $20 ; end of string mark + space

__msg_kernalfrom_hex:
	!pet  " FROM "
	!byte $80 + $24 ; end of string mark + '$'

__msg_kernalto_hex:
	!pet  " TO "
	!byte $80 + $24 ; end of string mark + '$'

!ifdef HAS_TAPE {

__msg_kernalplay:
	!byte $0D
	!pet  "PRESS PLAY ON TAPE"
	!byte $80 + $0D ; end of string mark + return

__msg_kernalfound:
	!pet  "FOUND"
	!byte $80 + $20 ; end of string mark + space

!ifdef CONFIG_MB_M65 {

__msg_kernaloksearching:
	!pet  "OK"
	!byte $0D, $0D
	!pet  "SEARCHIN"
	!byte $80 + $47 ; end of string mark + 'G'
} }

!ifdef CONFIG_PANIC_SCREEN {

__msg_kernalpanic:
	!pet  "KERNAL PANI"
	!byte $80 + $43 ; end of string mark + 'C'

__msg_kernalpanic_rom_mismatch:
	!pet  " - ROM MISMATC"
	!byte $80 + $48 ; end of string mark + 'H'
}
