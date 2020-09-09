
;
; Various configuration dependent aliases/macros/checks
;



; Check that platform is specified

!set counter = 0
!ifdef CONFIG_PLATFORM_COMMODORE_64  { !set counter = counter + 1 }
!ifdef CONFIG_PLATFORM_COMMANDER_X16 { !set counter = counter + 1 }

!if (counter != 1) { !error "Please select exactly one CONFIG_PLATFORM_* option" }



; Check that motherboard configuration is correct

!set counter = 0
!ifdef CONFIG_MB_M65 { !set counter = counter + 1 }
!ifdef CONFIG_MB_U64 { !set counter = counter + 1 }

!if (counter > 1) { !error "Please select at most one CONFIG_MB_* option" }

!ifdef ROM_LAYOUT_M65 { !ifndef CONFIG_MB_M65 { !error "ROM layout requires CONFIG_MB_M65 option" } }
!ifdef ROM_LAYOUT_U16 { !ifndef CONFIG_MB_U64 { !error "ROM layout requires CONFIG_MB_U64 option" } }

!ifdef CONFIG_MB_M65 { !ifndef ROM_LAYOUT_M65 { !error "CONFIG_MB_M65 can only be used with appropriate ROM layout" } }
; !ifdef CONFIG_MB_U64 { !ifndef ROM_LAYOUT_U64 { !error "CONFIG_MB_U64 can only be used with appropriate ROM layout" } }

!ifdef CONFIG_MB_M65 { !ifndef CONFIG_PLATFORM_COMMODORE_64 { !error "CONFIG_MB_M65 can only be used with CONFIG_PLATFORM_COMMODORE_64" } }
!ifdef CONFIG_MB_U64 { !ifndef CONFIG_PLATFORM_COMMODORE_64 { !error "CONFIG_MB_U64 can only be used with CONFIG_PLATFORM_COMMODORE_64" } }



; Check that brand configuration is correct

!set counter = 0
!ifdef CONFIG_BRAND_CUSTOM_BUILD { !set counter = counter + 1 }
!ifdef CONFIG_BRAND_GENERIC      { !set counter = counter + 1 }
!ifdef CONFIG_BRAND_TESTING      { !set counter = counter + 1 }

!ifndef CONFIG_PLATFORM_COMMODORE_64 {
	!if (counter != 0) { !error "Do not use CONFIG_BRAND_* options for non-C64 platforms" }
} else ifdef CONFIG_MB_M65 {
	!if (counter != 0) { !error "Do not use CONFIG_BRAND_* options for the MEGA65 motherboard" }
} else ifdef CONFIG_MB_MU64 {
	!if (counter != 0) { !error "Do not use CONFIG_BRAND_* options for the Ultimate 64 motherboard" }
} else if (counter != 1) {
	!error "Please select exactly one CONFIG_BRAND_* option"	
}



; Check that processor configuration is correct

!set counter = 0
!ifdef CONFIG_CPU_MOS_6502   { !set counter = counter + 1 }
!ifdef CONFIG_CPU_DTV_6502   { !set counter = counter + 1 }
!ifdef CONFIG_CPU_RCW_65C02  { !set counter = counter + 1 }
!ifdef CONFIG_CPU_WDC_65C02  { !set counter = counter + 1 }
!ifdef CONFIG_CPU_WDC_65816  { !set counter = counter + 1 }
!ifdef CONFIG_CPU_CSG_65CE02 { !set counter = counter + 1 }

!ifdef CONFIG_PLATFORM_COMMANDER_X16 {
	!if (counter != 0) { !error "Do not use CONFIG_CPU_* options for Commander X16 platform" }
	!set CONFIG_CPU_WDC_65C02 = 1
} else ifdef CONFIG_MB_M65 {
	!if (counter != 0) { !error "Do not use CONFIG_CPU_* options for MEGA65 motherboard" }
	!set CONFIG_CPU_CSG_65CE02 = 1
} else ifdef CONFIG_MB_U64 {
	!if (counter != 0) { !error "Do not use CONFIG_CPU_* options for Ultimate 64 motherboard" }
	!set CONFIG_CPU_MOS_6502 = 1
} else if (counter != 1) {
	!error "Please select exactly one CONFIG_CPU_* option"	
}



; Check that memory model configuration is correct

!set counter = 0
!ifdef CONFIG_MEMORY_MODEL_38K { !set counter = counter + 1 }
!ifdef CONFIG_MEMORY_MODEL_46K { !set counter = counter + 1 }
!ifdef CONFIG_MEMORY_MODEL_50K { !set counter = counter + 1 }
!ifdef CONFIG_MEMORY_MODEL_60K { !set counter = counter + 1 }

!ifdef CONFIG_MB_M65 {
	!ifdef CONFIG_MEMORY_MODEL_60K { !error "Do not use CONFIG_MEMORY_MODEL_60K options for MEGA65 motherboard" }
}
!ifdef CONFIG_PLATFORM_COMMANDER_X16 {
	!if (counter != 0) { !error "Do not use CONFIG_MEMORY_MODEL_* options for Commander X16 platform" }
	!set CONFIG_MEMORY_MODEL_38K = 1
} else if (counter != 1) {
	!error "Please select exactly one CONFIG_MEMORY_MODEL_* option"	
}



; Check that IEC configuration is correct

!ifdef CONFIG_PLATFORM_COMMANDER_X16 {
	!ifdef CONFIG_IEC { !error "Do not use CONFIG_IEC options for Commander X16 platform, it is not implemented" }
}
!ifdef CONFIG_MB_MEGA65 {
	!ifdef CONFIG_IEC_BURST_CIA1 { !error "Do not use CONFIG_IEC_BURST_CIA1 options for MEGA65 motherboard, CONFIG_IEC_BURST_M65 instead" }
	!ifdef CONFIG_IEC_BURST_CIA2 { !error "Do not use CONFIG_IEC_BURST_CIA2 options for MEGA65 motherboard, CONFIG_IEC_BURST_M65 instead" }
}
!ifndef CONFIG_MB_MEGA65 {
	!ifdef CONFIG_IEC_BURST_M65 { !error "CONFIG_IEC_BURST_M65 is only possible for MEGA65 motherboard" }
}
!ifdef CONFIG_IEC_BURST_CIA1 {
	!ifndef CONFIG_IEC { !error "CONFIG_IEC_BURST_CIA1 requires CONFIG_IEC" }
}
!ifdef CONFIG_IEC_BURST_CIA2 {
	!ifndef CONFIG_IEC { !error "CONFIG_IEC_BURST_CIA2 requires CONFIG_IEC" }
}
!ifdef CONFIG_IEC_BURST_CIA1 {
	!ifdef CONFIG_IEC_BURST_CIA2 { !error "CONFIG_IEC_BURST_CIA1 and CONFIG_IEC_BURST_CIA2 are mutually exclusive" }
}
!ifdef CONFIG_IEC_BURST_M65 {
	!ifndef CONFIG_IEC { !error "CONFIG_IEC_BURST_M65 requires CONFIG_IEC" }
}
!ifdef CONFIG_IEC_DOLPHINDOS {
	!ifndef CONFIG_IEC { !error "CONFIG_IEC_DOLPHINDOS requires CONFIG_IEC" }
}
!ifdef CONFIG_IEC_DOLPHINDOS_FAST {
	!ifndef CONFIG_IEC_DOLPHINDOS { !error "CONFIG_IEC_DOLPHINDOS_FAST requires CONFIG_IEC_DOLPHINDOS" }
}
!ifdef CONFIG_IEC_JIFFYDOS {
	!ifndef CONFIG_IEC { !error "CONFIG_IEC_JIFFYDOS requires CONFIG_IEC" }
}
!ifdef CONFIG_IEC_JIFFYDOS_BLANK {
	!ifndef CONFIG_IEC_JIFFYDOS { !error "CONFIG_IEC_JIFFYDOS_BLANK requires CONFIG_IEC_JIFFYDOS" }
}



; Check that tape deck configuration is correct

!set counter = 0
!ifdef CONFIG_TAPE_NORMAL { !set counter = counter + 1 }
!ifdef CONFIG_TAPE_TURBO  { !set counter = counter + 1 }

!ifdef CONFIG_PLATFORM_COMMANDER_X16 {
	!ifdef CONFIG_TAPE_NORMAL { !error "Do not use CONFIG_TAPE_NORMAL options for Commander X16 platform, it is not implemented" }
	!ifdef CONFIG_TAPE_TURBO  { !error "Do not use CONFIG_TAPE_TURBO options for Commander X16 platform, it is not implemented" }
}
!ifdef CONFIG_TAPE_AUTODETECT { !ifndef CONFIG_TAPE_NORMAL { !ifndef CONFIG_TAPE_TURBO {
	!error "CONFIG_TAPE_AUTODETECT requires both CONFIG_TAPE_NORMAL and CONFIG_TAPE_TURBO"
} } }
!ifdef CONFIG_TAPE_HEAD_ALIGN {
	!ifndef CONFIG_TAPE_WEDGE { !error "CONFIG_TAPE_HEAD_ALIGN requires CONFIG_TAPE_WEDGE" }
}
!ifdef CONFIG_TAPE_WEDGE {
	!if (counter = 0) { !error "CONFIG_TAPE_WEDGE requires CONFIG_TAPE_TURBO or CONFIG_TAPE_NORMAL"}
}



; Check that RS-232 configuration is correct

!set counter = 0
!ifdef CONFIG_RS232_ACIA   { !set counter = counter + 1 }
!ifdef CONFIG_RS232_UP2400 { !set counter = counter + 1 }
!ifdef CONFIG_RS232_UP9600 { !set counter = counter + 1 }

!ifdef CONFIG_PLATFORM_COMMANDER_X16 {
	!if (counter != 0) { !error "Do not use CONFIG_RS232_* options for Commander X16 platform, it is not implemented" }
}
!ifdef CONFIG_RS232_UP9600 {
	!ifdef CONFIG_TAPE_NORMAL { !error "CONFIG_RS232_UP9600 is not compatible with CONFIG_TAPE_NORMAL" }
	!ifdef CONFIG_TAPE_TURBO  { !error "CONFIG_RS232_UP9600 is not compatible with CONFIG_TAPE_TURBO"  }
}
!if (counter > 1) { !error "Please select at most one CONFIG_RS232_* option" }



; Check that sound support configuration is correct

!set counter = 0
!ifdef CONFIG_SID_2ND_ADDRESS { !set counter = counter + 1 }
!ifdef CONFIG_SID_3RD_ADDRESS { !set counter = counter + 1 }
!ifdef CONFIG_SID_D4XX        { !set counter = counter + 1 }
!ifdef CONFIG_SID_D5XX        { !set counter = counter + 1 }

!ifdef CONFIG_PLATFORM_COMMANDER_X16 {
	!if (counter != 0) { !error "Do not use CONFIG_SID_* options for Commander X16 platform" }
}
!ifdef CONFIG_MB_M65 {
	!if (counter != 0) { !error "Do not use CONFIG_SID_* options for MEGA65 motherboard" }
}
!ifdef CONFIG_SID_2ND_ADDRESS {
	!if (CONFIG_SID_2ND_ADDRESS = $D400) { !error "CONFIG_SID_2ND_ADDRESS points to the 1st SID" }
}
!ifdef CONFIG_SID_3RD_ADDRESS {
	!if (CONFIG_SID_3RD_ADDRESS = $D400) { !error "CONFIG_SID_3RD_ADDRESS points to the 1st SID" }
}
!ifdef CONFIG_SID_2ND_ADDRESS { !ifdef CONFIG_SID_3RD_ADDRESS {
	!if (CONFIG_SID_2ND_ADDRESS = CONFIG_SID_RD_ADDRESS) { !error "Configured SIDs have the same addresses" }
} }



; Check that keyboard settings are correct

!ifdef CONFIG_LEGACY_SCNKEY {
	!ifdef CONFIG_RS232_UP2400            { !error "CONFIG_LEGACY_SCNKEY is not compatible with CONFIG_RS232_UP2400"               }
	!ifdef CONFIG_RS232_UP9600            { !error "CONFIG_LEGACY_SCNKEY is not compatible with CONFIG_RS232_UP9600"               }
	!ifdef CONFIG_TAPE_NORMAL             { !error "CONFIG_LEGACY_SCNKEY is not compatible with CONFIG_TAPE_NORMAL"                }
	!ifdef CONFIG_KEYBOARD_C128           { !error "CONFIG_LEGACY_SCNKEY is not compatible with CONFIG_KEYBOARD_C128"              }
	!ifdef CONFIG_KEYBOARD_C128_CAPS_LOCK { !error "CONFIG_LEGACY_SCNKEY is not compatible with CONFIG_KEYBOARD_C128_CAPS_LOCK"    }
	!ifdef CONFIG_KEYBOARD_C65            { !error "CONFIG_LEGACY_SCNKEY is not compatible with CONFIG_KEYBOARD_C65"               }
	!ifdef CONFIG_KEYBOARD_C65_CAPS_LOCK  { !error "CONFIG_LEGACY_SCNKEY is not compatible with CONFIG_KEYBOARD_C65_CAPS_LOCK"     }
}
!ifdef CONFIG_MB_M65 {
	!ifdef CONFIG_KEYBOARD_C128           { !error "MEGA65 motherboard is not compatible with CONFIG_KEYBOARD_C128"                }
	!ifdef CONFIG_KEYBOARD_C128_CAPS_LOCK { !error "MEGA65 motherboard is not compatible with CONFIG_KEYBOARD_C128_CAPS_LOCK"      }
	!ifdef CONFIG_LEGACY_SCNKEY           { !error "MEGA65 motherboard is not compatible with CONFIG_LEGACY_SCNKEY"                }
}
!ifdef CONFIG_MB_U64 {
	!ifdef CONFIG_KEYBOARD_C128           { !error "Ultimate 64 motherboard is not compatible with CONFIG_KEYBOARD_C128"           }
	!ifdef CONFIG_KEYBOARD_C128_CAPS_LOCK { !error "Ultimate 64 motherboard is not compatible with CONFIG_KEYBOARD_C128_CAPS_LOCK" }
	!ifdef CONFIG_KEYBOARD_C65            { !error "Ultimate 64 motherboard is not compatible with CONFIG_KEYBOARD_C65"            }
	!ifdef CONFIG_KEYBOARD_C65_CAPS_LOCK  { !error "Ultimate 64 motherboard is not compatible with CONFIG_KEYBOARD_C65_CAPS_LOCK"  }
}



; Check that startup banner configuration is correct

!set counter = 0
!ifdef CONFIG_BANNER_SIMPLE { !set counter = counter + 1 }
!ifdef CONFIG_BANNER_FANCY  { !set counter = counter + 1 }

!ifdef CONFIG_MB_M65 {
	!if (counter > 0) { !error "Do not use CONFIG_BANNER_* options for MEGA65"    }
	!ifdef CONFIG_COLORS_BRAND  { !errror "Do not use CONFIG_COLORS_BRAND options for MEGA65" }
	!ifdef CONFIG_SHOW_FEATURES { !errror "Do not use CONFIG_SHOW_FEATURES options for MEGA65" }
} else {
	!if (counter != 1) { !error "Please select exactly one CONFIG_BANNER_* option" }
}



;
; Define some macros depending on the configuration
;



; Handle CPU configuration

!ifdef CONFIG_BCD_SAFE_INTERRUPTS { !set HAS_BCD_SAFE_INTERRUPTS = 1 }



; Handle RS-232 configuration

!ifdef CONFIG_RS232_ACIA   { !set HAS_RS232 = 1 }
!ifdef CONFIG_RS232_UP2400 { !set HAS_RS232 = 1 }
!ifdef CONFIG_RS232_UP9600 { !set HAS_RS232 = 1 }



; Handle debug configuration

!macro STUB_IMPLEMENTATION_RTS { 
	rts
}

!macro STUB_IMPLEMENTATION_BRK { 
	; XXX set VICE breakpoint
	brk
	brk
	jmp *-2
}

!ifdef CONFIG_DBG_STUBS_BRK {
	!macro STUB_IMPLEMENTATION { +STUB_IMPLEMENTATION_BRK }
} else {
	!macro STUB_IMPLEMENTATION { +STUB_IMPLEMENTATION_RTS }
}



; Handle colors

!set CONFIG_COLOR_BG  = $06
!set CONFIG_COLOR_TXT = $01

!ifdef CONFIG_COLORS_BRAND {
	!ifdef CONFIG_MB_U64 {
		!set CONFIG_COLOR_BG  = $00
		!set CONFIG_COLOR_TXT = $01
	}
}



; Determine if we need space-savings in BASIC code

!ifndef CONFIG_MB_M65 {
	!set HAS_SMALL_BASIC = 1
}
