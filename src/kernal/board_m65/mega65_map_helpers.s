;; #LAYOUT# M65 KERNAL_0 #TAKE
;; #LAYOUT# *   *        #IGNORE

;
; ROM mapping routines for MEGA65
;

; Available memory maps:
; - NORMAL            - nothing mapped in
; - KERNAL_1          - for calling KERNAL_1 segment code
; - DOS_1             - for swiching to DOS context
; - NORMAL_from_DOS_1 - for switching bact from DOS context
; - MON_1             - for the monitor
; - ZVM_1             - for Z80 virtual machine


map_NORMAL:

	php
	pha
	phx
	phy
	phz

	lda #$00

	; FALLTROUGH

map_NORMAL_common:

	tax
	tay
	taz

	; FALLTROUGH

map_end:

	map
	eom

	; FALLTROUGH

map_end_no_eom:

	plz
	ply
	plx
	pla
	plp

	rts


map_KERNAL_1:

	php
	pha
	phx
	phy
	phz

	lda #$00
	tay
	taz

	ldx #$42                 ; $4000 <- map 8KB from $24000

	bra map_end

map_DOS_1:

	php
	pha
	phx
	phy
	phz

	ldy #$80
	ldz #$30                 ; $8000 <- map 16KB RAM from $10000

	lda #$C0
	ldx #$C1                 ; $4000 <- map 16KB ROM from $20000

	map
	bra map_end_no_eom       ; no EOM, we do not want interrupts within DOS!

map_MON_1:

	php
	pha
	phx
	phy
	phz

	lda #$00
	tay
	taz

	lda #$C0
	ldx #$42                 ; $4000 <- map 8KB from $30000

	map
	bra map_end

map_ZVM_1:

	php
	pha
	phx
	phy
	phz

	lda #$00
	tay
	taz

	ldx #$E3                 ; $2000 <- map 24KB from $32000

	map
	bra map_end
