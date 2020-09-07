
!set CONFIG_ID = $02 ; should be 1 byte, different for each config file!

;
; Please read CONFIG.md before modifying this file!
;

; Idea:
; - sane defaults for the Mega 65 FPGA computer
; - do not enable features which are a significant compatibility risk



; Hardware platform

!set CONFIG_PLATFORM_COMMODORE_64 = 1
!set CONFIG_MB_MEGA_65 = 1

; Processor instruction set

; !set CONFIG_CPU_MOS_6502 = 1
; !set CONFIG_CPU_WDC_65C02 = 1
; !set CONFIG_CPU_CSG_65CE02 = 1
!set CONFIG_CPU_M65_45GS02 = 1
; !set CONFIG_CPU_WDC_65816 = 1


; Memory model

; !set CONFIG_MEMORY_MODEL_38K = 1
; !set CONFIG_MEMORY_MODEL_46K = 1
!set CONFIG_MEMORY_MODEL_50K = 1
; !set CONFIG_MEMORY_MODEL_60K = 1


; I/O devices

!set CONFIG_IEC = 1
; !set CONFIG_IEC_DOLPHINDOS = 1
; !set CONFIG_IEC_DOLPHINDOS_FAST = 1
!set CONFIG_IEC_JIFFYDOS = 1
!set CONFIG_IEC_JIFFYDOS_BLANK = 1

!set CONFIG_TAPE_NORMAL = 1
!set CONFIG_TAPE_TURBO = 1
!set CONFIG_TAPE_AUTODETECT = 1
!set CONFIG_TAPE_NO_KEY_SENSE = 1
!set CONFIG_TAPE_NO_MOTOR_CONTROL = 1


; Multiple SID support

; !set CONFIG_SID_2ND
!set CONFIG_SID_2ND_ADDRESS = $D440

; !set CONFIG_SID_3RD
!set CONFIG_SID_3RD_ADDRESS = $D480

; !set CONFIG_SID_D4XX = 1
; !set CONFIG_SID_D5XX = 1


; Keyboard settings

; !set CONFIG_LEGACY_SCNKEY = 1
; !set CONFIG_KEYBOARD_C128 = 1
; !set CONFIG_KEYBOARD_C128_CAPS_LOCK = 1
; !set CONFIG_KEYBOARD_C65 = 1              ; untested
; !set CONFIG_KEYBOARD_C65_CAPS_LOCK = 1    ; untested
; !set CONFIG_KEY_REPEAT_DEFAULT = 1
; !set CONFIG_KEY_REPEAT_ALWAYS = 1
!set CONFIG_KEY_FAST_SCAN = 1
!set CONFIG_JOY1_CURSOR = 1
!set CONFIG_JOY2_CURSOR = 1

!set CONFIG_PROGRAMMABLE_KEYS = 1

.const !set CONFIG_KEYCMD_RUN  = @"\$5FL"

.const !set CONFIG_KEYCMD_F1   = @"@"
.const !set CONFIG_KEYCMD_F2   = @""
.const !set CONFIG_KEYCMD_F3   = @"RUN:"
.const !set CONFIG_KEYCMD_F4   = @""
.const !set CONFIG_KEYCMD_F5   = @"LOAD"
.const !set CONFIG_KEYCMD_F6   = @""
.const !set CONFIG_KEYCMD_F7   = @"@$"
.const !set CONFIG_KEYCMD_F8   = @""

.const !set CONFIG_KEYCMD_HELP = @"LIST"

.const !set CONFIG_KEYCMD_F9   = @"BOOT"
.const !set CONFIG_KEYCMD_F10  = @"REM F10"
.const !set CONFIG_KEYCMD_F11  = @"MONITOR"
.const !set CONFIG_KEYCMD_F12  = @"REM F12"
.const !set CONFIG_KEYCMD_F13  = @"\$5FH"
.const !set CONFIG_KEYCMD_F14  = @"REM F14"


; Screen editor

!set CONFIG_EDIT_STOPQUOTE = 1
!set CONFIG_EDIT_TABULATORS = 1


; Software features

!set CONFIG_PANIC_SCREEN = 1
!set CONFIG_DOS_WEDGE = 1
!set CONFIG_TAPE_WEDGE = 1
!set CONFIG_TAPE_HEAD_ALIGN = 1
!set CONFIG_BCD_SAFE_INTERRUPTS = 1


; Built-in DOS configuration

!set CONFIG_UNIT_SDCARD  = 0 ; do not change, DOS is not implemented yet
!set CONFIG_UNIT_FLOPPY  = 0 ; do not change, DOS is not implemented yet
!set CONFIG_UNIT_RAMDISK = 0 ; do not change, DOS is not implemented yet


; Debug options

; !set CONFIG_DBG_STUBS_BRK = 1
; !set CONFIG_DBG_PRINTF = 1

; Other

; !set CONFIG_COMPRESSION_LVL_2 = 1
