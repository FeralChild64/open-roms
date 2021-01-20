;; #LAYOUT# M65 KERNAL_0 #TAKE
;; #LAYOUT# *   *        #IGNORE

;
; Proxies for calling MEGA65 segment KERNAL_0 routines from MON_1
;


proxy_M1_CHRIN:

	jsr map_NORMAL
	jsr CHRIN
	bra proxy_M1_end

proxy_M1_CHROUT:

	jsr map_NORMAL
	jsr CHROUT

	; FALLTROUGH

proxy_M1_jmpout_ret:         ; reused by monitor
proxy_M1_end:

	jmp map_MON_1

proxy_M1_JLOAD:

	jsr map_NORMAL
	jsr JLOAD
	bra proxy_M1_end

proxy_M1_JSAVE:

	jsr map_NORMAL
	jsr JSAVE
	bra proxy_M1_end

; Memory acccess routines


!addr Long__TMP = $64        ; has to be the same as Long_TMP in monitor

proxy_M1_memread:

	jsr map_NORMAL
	lda [Long__TMP], z
	bra proxy_M1_end

proxy_M1_memwrite:

	jsr map_NORMAL
	sta [Long__TMP], z
	bra proxy_M1_end


; JMP/JSR to user code

proxy_M1_jmpout:

	plz  ; ZR
	ply  ; YR
	plx  ; XR
	pla  ; BP
	tab
	pla  ; AC
	plp  ; SR

	jmp map_NORMAL
