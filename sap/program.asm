.include "asm/m328Pdef.inc"

; Zacatek programu - po resetu
.org 0
jmp start

; Zacatek programu - hlavni program
start:
    ldi r16, 42
    ldi r17, 100
    
end: rjmp end ; Zastavime program - nekonecna smycka 3
