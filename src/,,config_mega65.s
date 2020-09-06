
CONFIG_ID = $02 ; should be 1 byte, different for each config file!

;
; Please read CONFIG.md before modifying this file!
;

; Idea:
; - sane defaults for the Mega 65 FPGA computer
; - do not enable features which are a significant compatibility risk



; Hardware platform

CONFIG_PLATFORM_COMMODORE_64 = 1
CONFIG_MB_MEGA_65 = 1

; Processor instruction set

; CONFIG_CPU_MOS_6502 = 1
; CONFIG_CPU_WDC_65C02 = 1
; CONFIG_CPU_CSG_65CE02 = 1
CONFIG_CPU_M65_45GS02 = 1
; CONFIG_CPU_WDC_65816 = 1


; Memory model

; CONFIG_MEMORY_MODEL_38K = 1
; CONFIG_MEMORY_MODEL_46K = 1
CONFIG_MEMORY_MODEL_50K = 1
; CONFIG_MEMORY_MODEL_60K = 1


; I/O devices

CONFIG_IEC = 1
; CONFIG_IEC_DOLPHINDOS = 1
; CONFIG_IEC_DOLPHINDOS_FAST = 1
CONFIG_IEC_JIFFYDOS = 1
CONFIG_IEC_JIFFYDOS_BLANK = 1

CONFIG_TAPE_NORMAL = 1
CONFIG_TAPE_TURBO = 1
CONFIG_TAPE_AUTODETECT = 1
CONFIG_TAPE_NO_KEY_SENSE = 1
CONFIG_TAPE_NO_MOTOR_CONTROL = 1


; Multiple SID support

; CONFIG_SID_2ND
CONFIG_SID_2ND_ADDRESS = $D440

; CONFIG_SID_3RD
CONFIG_SID_3RD_ADDRESS = $D480

; CONFIG_SID_D4XX = 1
; CONFIG_SID_D5XX = 1


; Keyboard settings

; CONFIG_LEGACY_SCNKEY = 1
; CONFIG_KEYBOARD_C128 = 1
; CONFIG_KEYBOARD_C128_CAPS_LOCK = 1
; CONFIG_KEYBOARD_C65 = 1              ; untested
; CONFIG_KEYBOARD_C65_CAPS_LOCK = 1    ; untested
; CONFIG_KEY_REPEAT_DEFAULT = 1
; CONFIG_KEY_REPEAT_ALWAYS = 1
CONFIG_KEY_FAST_SCAN = 1
CONFIG_JOY1_CURSOR = 1
CONFIG_JOY2_CURSOR = 1

CONFIG_PROGRAMMABLE_KEYS = 1

.const CONFIG_KEYCMD_RUN  = @"\$5FL"

.const CONFIG_KEYCMD_F1   = @"@"
.const CONFIG_KEYCMD_F2   = @""
.const CONFIG_KEYCMD_F3   = @"RUN:"
.const CONFIG_KEYCMD_F4   = @""
.const CONFIG_KEYCMD_F5   = @"LOAD"
.const CONFIG_KEYCMD_F6   = @""
.const CONFIG_KEYCMD_F7   = @"@$"
.const CONFIG_KEYCMD_F8   = @""

.const CONFIG_KEYCMD_HELP = @"LIST"

.const CONFIG_KEYCMD_F9   = @"BOOT"
.const CONFIG_KEYCMD_F10  = @"REM F10"
.const CONFIG_KEYCMD_F11  = @"MONITOR"
.const CONFIG_KEYCMD_F12  = @"REM F12"
.const CONFIG_KEYCMD_F13  = @"\$5FH"
.const CONFIG_KEYCMD_F14  = @"REM F14"


; Screen editor

CONFIG_EDIT_STOPQUOTE = 1
CONFIG_EDIT_TABULATORS = 1


; Software features

CONFIG_PANIC_SCREEN = 1
CONFIG_DOS_WEDGE = 1
CONFIG_TAPE_WEDGE = 1
CONFIG_TAPE_HEAD_ALIGN = 1
CONFIG_BCD_SAFE_INTERRUPTS = 1


; Built-in DOS configuration

CONFIG_UNIT_SDCARD  = 0 ; do not change, DOS is not implemented yet
CONFIG_UNIT_FLOPPY  = 0 ; do not change, DOS is not implemented yet
CONFIG_UNIT_RAMDISK = 0 ; do not change, DOS is not implemented yet


; Debug options

; CONFIG_DBG_STUBS_BRK = 1
; CONFIG_DBG_PRINTF = 1

; Other

; CONFIG_COMPRESSION_LVL_2 = 1
