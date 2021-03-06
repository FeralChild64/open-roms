;; #LAYOUT# STD *       #TAKE
;; #LAYOUT# *   BASIC_0 #TAKE
;; #LAYOUT# *   *       #IGNORE

;
; Fetches address for BLOAD/BVERIFY/BSAVE, reports error if not found
;


!ifndef HAS_SMALL_BASIC {

helper_bload_fetch_address:

	jsr injest_comma
	+bcs do_SYNTAX_error

	lda #IDX__EV2_0B ; 'SYNTAX ERROR'
	jsr fetch_uint16
	+bcs do_SYNTAX_error

	ldx LINNUM+0
	ldy LINNUM+1

	rts
}