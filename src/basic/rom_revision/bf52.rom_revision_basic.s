;; #LAYOUT# STD *       #TAKE
;; #LAYOUT# X16 BASIC_0 #TAKE-OFFSET 2000
;; #LAYOUT# *   BASIC_0 #TAKE
;; #LAYOUT# *   *       #IGNORE

;
; !!! PROPOSAL ONLY !!! PROPOSAL ONLY !!! NOT FOR USING EXTERNALLY YET !!!
;
;
; Do not change! Locations of the following data should be constant - now and forever!
;
; If you want to integrate Open ROMs support in your emulator, FPGA ccomputer, etc. - this
; is the official way to recognize the ROM and its revision.
;


rom_revision_basic:

	; $BF52

	.text "OR"                ; project signature, after "Open ROMs"
	.byte CONFIG_ID           ; config file ID, might change between revisions

rom_revision_basic_string:

	; $BF55

#if !CONFIG_BRAND_CUSTOM_BUILD
	.text "(DEVEL SNAPSHOT)"  ; actual ROM revision string; up to 16 characters
#else
	.text "(CUSTOM BUILD)"
#endif

	.byte $00                 ; marks the end of string

__rom_revision_basic_end:
