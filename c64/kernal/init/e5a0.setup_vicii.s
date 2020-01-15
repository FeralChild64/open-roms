// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE

//
// Well-known Kernal routine, described in:
//
// - [CM64] Computes Mapping the Commodore 64 - page 215
//


setup_vicii:

	// Clear everything - turn off sprites (observed hanging around after
 	// running programs and resetting), etc.

	lda #$00
 	ldx #$2E
 !:
 	sta __VIC_BASE, x
 	dex
 	bpl !-

#if !CONFIG_MB_MEGA_65 && !CONFIG_MB_ULTIMATE_64

 	// Disable C128 extra keys - just to be sure they will not interfere with anything

 	stx VIC_XSCAN

 	// Disable the C128 2MHz mode, it prevents VIC-II display from working correctly

 	stx VIC_CLKRATE

#endif

	// Set up default IO values
	// - [CM64] page 129       - VIC_SCROLY
	// - [CM64] pages 140-144  - VIC_SCROLX
	// - [CM64] pages 145-146  - VIC_YMCSB

	lda #$1B                           // according to [CM64] page 137 some software assumes oldest bit is 0
	sta VIC_SCROLY
	lda #$C8
	sta VIC_SCROLX                     // 40 column etc
	lda #$14
	sta VIC_YMCSB
	lda #$0F
	sta VIC_IRQ                        // clear all interrupts

	// We use a different colour scheme of white text on all blue by default,
	// but brand-specific color scheme can also be enabled

#if CONFIG_COLORS_BRAND && CONFIG_BRAND_ULTIMATE_64

	lda #$00
	sta VIC_EXTCOL
	sta VIC_BGCOL0

#else

	lda #$06
	sta VIC_EXTCOL
	sta VIC_BGCOL0

#endif

	// Setup default I/O devices

	jmp clrchn_reset
