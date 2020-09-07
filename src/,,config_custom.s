
!set CONFIG_ID = $00 ; should be 1 byte, different for each config file!

;
; Please read CONFIG.md before modifying this file!
;

; Idea:
; - configuration for custom builds and for development
; - file contains options to enable unfinished functionalities



; Hardware platform and brand

!set CONFIG_PLATFORM_COMMODORE_64 = 1

!set CONFIG_BRAND_CUSTOM_BUILD = 1
; !set CONFIG_BRAND_GENERIC = 1
; !set CONFIG_BRAND_TESTING = 1
; !set CONFIG_BRAND_ULTIMATE_64 = 1

.const !set CONFIG_CUSTOM_BRAND = @"CUSTOM BUILD"


; Processor instruction set

!set CONFIG_CPU_MOS_6502 = 1
; !set CONFIG_CPU_WDC_65C02 = 1
; !set CONFIG_CPU_CSG_65CE02 = 1
; !set CONFIG_CPU_M65_45GS02 = 1
; !set CONFIG_CPU_WDC_65816 = 1


; Memory model

; !set CONFIG_MEMORY_MODEL_38K = 1
; !set CONFIG_MEMORY_MODEL_46K = 1
!set CONFIG_MEMORY_MODEL_50K = 1
; !set CONFIG_MEMORY_MODEL_60K = 1


; I/O devices

!set CONFIG_IEC = 1
!set CONFIG_IEC_DOLPHINDOS = 1
!set CONFIG_IEC_DOLPHINDOS_FAST = 1
!set CONFIG_IEC_JIFFYDOS = 1
; !set CONFIG_IEC_JIFFYDOS_BLANK = 1
; !set CONFIG_IEC_BURST_CIA1 = 1                 ; please keep disabled for now
; !set CONFIG_IEC_BURST_CIA2 = 1                 ; please keep disabled for now
; !set CONFIG_IEC_BURST_MEGA_65 = 1              ; please keep disabled for now

!set CONFIG_TAPE_NORMAL = 1
!set CONFIG_TAPE_TURBO = 1
!set CONFIG_TAPE_AUTODETECT = 1
; !set CONFIG_TAPE_NO_KEY_SENSE = 1
; !set CONFIG_TAPE_NO_MOTOR_CONTROL = 1

; !set CONFIG_RS232_UP2400 = 1                   ; please keep disabled for now
; !set CONFIG_RS232_UP9600 = 1                   ; please keep disabled for now


; Multiple SID support

; !set CONFIG_SID_2ND = 1
!set CONFIG_SID_2ND_ADDRESS = $D420

; !set CONFIG_SID_3RD = 1 
!set CONFIG_SID_3RD_ADDRESS = $D440

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
; !set CONFIG_JOY1_CURSOR = 1
; !set CONFIG_JOY2_CURSOR = 1

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

.const !set CONFIG_KEYCMD_F9   = @""
.const !set CONFIG_KEYCMD_F10  = @""
.const !set CONFIG_KEYCMD_F11  = @""
.const !set CONFIG_KEYCMD_F12  = @""
.const !set CONFIG_KEYCMD_F13  = @""
.const !set CONFIG_KEYCMD_F14  = @""


; Screen editor

!set CONFIG_EDIT_STOPQUOTE = 1
; !set CONFIG_EDIT_TABULATORS = 1


; Software features

!set CONFIG_PANIC_SCREEN = 1
!set CONFIG_DOS_WEDGE = 1
!set CONFIG_TAPE_WEDGE = 1
; !set CONFIG_TAPE_HEAD_ALIGN = 1
!set CONFIG_BCD_SAFE_INTERRUPTS = 1

; Eye candy

; !set CONFIG_COLORS_BRAND = 1
; !set CONFIG_BANNER_SIMPLE = 1
!set CONFIG_BANNER_FANCY = 1
!set CONFIG_SHOW_FEATURES = 1


; Debug options

; !set CONFIG_DBG_STUBS_BRK = 1
; !set CONFIG_DBG_PRINTF = 1

; Other

; !set CONFIG_COMPRESSION_LVL_2 = 1
