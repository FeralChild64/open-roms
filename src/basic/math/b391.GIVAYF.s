;; #LAYOUT# STD *       #TAKE
;; #LAYOUT# X16 BASIC_0 #TAKE-OFFSET 2000
;; #LAYOUT# *   BASIC_0 #TAKE
;; #LAYOUT# *   *       #IGNORE

;
; Math package - 16-bit signed integer to FAC1
;
; Output:
; - .A - high byte
; - .Y - low byte
;
; Sets the data type flag at $0D to "numeric", stores the number in the FAC1 mantissa
; then calls convert_i16_to_FAC1
;
; See also:
; - [CM64] Computes Mapping the Commodore 64 - page 107
; - https://www.c64-wiki.com/wiki/GIVAYF
;


GIVAYF:
    ldx #$00
    stx $0D
    sty FAC1_mantissa
    sta FAC1_mantissa+1
    jmp convert_i16_to_FAC1
