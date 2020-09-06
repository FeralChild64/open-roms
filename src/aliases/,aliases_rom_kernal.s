;; #LAYOUT# * BASIC_1 #IGNORE
;; #LAYOUT# * *       #TAKE

	; Kernal jump table

	.label JCINT    = $FF81
	.label JIOINIT  = $FF84
	.label JRAMTAS  = $FF87
	.label JRESTOR  = $FF8A
	.label JVECTOR  = $FF8D
	.label JSETMSG  = $FF90
	.label JSECOND  = $FF93
	.label JTKSA    = $FF96
	.label JMEMTOP  = $FF99
	.label JMEMBOT  = $FF9C
	.label JSCNKEY  = $FF9F
	.label JSETTMO  = $FFA2
	.label JACPTR   = $FFA5
	.label JCIOUT   = $FFA8
	.label JUNTLK   = $FFAB
	.label JUNLSN   = $FFAE
	.label JLISTEN  = $FFB1
	.label JTALK    = $FFB4
	.label JREADST  = $FFB7
	.label JSETFLS  = $FFBA
	.label JSETNAM  = $FFBD
	.label JOPEN    = $FFC0
	.label JCLOSE   = $FFC3
	.label JCHKIN   = $FFC6
	.label JCKOUT   = $FFC9
	.label JCLRCHN  = $FFCC
	.label JCHRIN   = $FFCF
	.label JCHROUT  = $FFD2
	.label JLOAD    = $FFD5
	.label JSAVE    = $FFD8
	.label JSETTIM  = $FFDB
	.label JRDTIM   = $FFDE
	.label JSTOP    = $FFE1
	.label JGETIN   = $FFE4
	.label JCLALL   = $FFE7
	.label JUDTIM   = $FFEA
	.label JSCREEN  = $FFED
	.label JPLOT    = $FFF0
	.label JIOBASE  = $FFF3
