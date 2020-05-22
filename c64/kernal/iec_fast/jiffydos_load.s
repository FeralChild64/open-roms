// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE

//
// JiffyDOS protocol support for IEC - optimized load loop
//
// Based on description from https://github.com/mist64/cbmbus_doc
//

#if CONFIG_IEC_JIFFYDOS && !CONFIG_MEMORY_MODEL_60K


// Note: original JiffyDOS LOAD loop checks for RUN/STOP key every time a sector is read,
// see description at https://sites.google.com/site/h2obsession/CBM/C128/JiffySoft128
//
// For simplicity and some space savings, our routine does not check the RUN/STOP key at all;
// the protocol is fast enough (especially with modern flash mediums) that probably nobody
// will want to terminate the loading.


jiffydos_load:

	jsr UNTLK

	lda FA
	jsr TALK
	// XXX bcs load_iec_err

	lda #$61 // ask to talk on channel 1
	sta TBTCNT
	jsr iec_tx_command
	// XXX bcs load_iec_error
	// XXX if not on JiffyDOS protoccol, report error

	// Timing is critical, do not allow interrupts
	sei

#if CONFIG_IEC_JIFFYDOS_BLANK

	// Preserve register with screen status (blank/visible)
	lda VIC_SCROLY
	sta TBTCNT

	// Blank screen to make sure no sprite/badline will interrupt
	jsr screen_off

#else

	// Store previous sprite status in temporary variable
	jsr jiffydos_prepare
	sta TBTCNT

#endif

	// Preserve 3 lowest bits of CIA2_PRA  XXX deduplicate this

	lda CIA2_PRA
	and #%00000111
	sta C3PO

	// A trick to shorten EAL update time
	ldy #$FF

	// FALLTROUGH

jiffydos_load_mode_escape:

	// We are now in escape mode - release DATA so that we can read it
	jsr iec_release_clk_data // XXX maybe release just data?

	// Wait till device tells us whether this is EOI/error
	jsr iec_wait_for_clk_release

	// Check DATA line - if pulled, there is more data
	bit CIA2_PRA
	bpl jiffydos_load_mode_receive

	// XXX check for error

	nop
.break
	nop

jiffydos_load_mode_receive:

	// XXX check whether there is more data to read (if CLK is pulled, switch back to escape mode)

	// XXX read data

	// Prepare 'start sending' message
	lda CIA2_PRA
	and #$FF - BIT_CIA2_PRA_DAT_OUT    // release

#if CONFIG_IEC_JIFFYDOS_BLANK

	// It seems JiffyDOS needs some time here; waste few cycles      XXX test this with new load loop
	nop
	jsr iec_wait_rts

	// Ask device to start sending bits
	sta CIA2_PRA

#else

	// Wait for appropriate moment, ask device to start sending bits
	tax
	jsr jiffydos_wait_line
	stx CIA2_PRA                       // cycles: 4

#endif
.break
	// Prepare 'data pull' byte, cycles: 3 + 2 + 2 = 7
	lda C3PO
	ora #BIT_CIA2_PRA_DAT_OUT          // pull
	tax

	// Delay, JiffyDOS needs some time, 4 cycles
	nop
	nop

	// Get bits, cycles: 4 + 2 + 2 = 8
	lda CIA2_PRA                       // bits 0 and 1 on CLK/DATA
	lsr
	lsr

	// Delay, JiffyDOS needs some time, 2 cycles
	nop

	// Get bits, cycles: 4 + 2 + 2 = 8
	ora CIA2_PRA                       // bits 2 and 3 on CLK/DATA
	lsr
	lsr

	// Get bits, cycles: 3 + 4 + 2 + 2 = 11
	eor C3PO
	eor CIA2_PRA                       // bits 4 and 5 on CLK/DATA
	lsr
	lsr

	// Get bits, cycles: 3 + 4 = 7
	eor C3PO
	eor CIA2_PRA                       // bits 6 and 7 on CLK/DATA

	// Store retrieved byte, cycles: 2 + 6 = 8 
	iny                                // NTSC needs this delay
	sta (EAL),y                        // not compatible with CONFIG_MEMORY_MODEL_60K


.break


xxx:
	jmp xxx







/* old version


jiffydos_load_loop:


	// Retrieve status bits, cycles: 4
	bit CIA2_PRA

	// Pull DATA at the end, cycles: 4
	stx CIA2_PRA

	// If CLK line not active - this was the last byte
	bvs jiffydos_load_end

	// Handle EAL
	cpy #$FF
	bne jiffydos_load_loop
	inc EAL+1
	jmp jiffydos_load_loop

jiffydos_load_end:

	// Save last byte
	tax

	// Update EAL
	sec
	jsr iec_update_EAL_by_Y

	// Indicate that no byte waits in output buffer
	lda #$00
	sta C3PO

	// Restore proper IECPROTO value
	lda #$01
	sta IECPROTO

#if CONFIG_IEC_JIFFYDOS_BLANK

	// Restore screen state
	lda TBTCNT
	sta VIC_SCROLY

#else

	// Re-enable sprites
	lda TBTCNT
	sta VIC_SPENA

#endif

	// Store last byte as unoptimized LOAD routine would
	stx TBTCNT

	// No need to re-enable interrupts; other IEC routines
	// called by LOAD will do this nevertheless

	// End of load loop
	jmp load_iec_loop_end

*/

#endif // CONFIG_IEC_JIFFYDOS and not CONFIG_MEMORY_MODEL_60K
