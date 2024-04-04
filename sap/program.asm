.include "asm/m328Pdef.inc"

; Zacatek programu - po resetu
.org 0
    jmp start

; podprogramy pro praci s displejem
.include "asm/printlib.inc"

; Zacatek programu - hlavni program
start:
    ; Inicializace displeje
    call init_disp
    
    ; *** ZDE muzeme psat nase instrukce
    ldi r16, '0'    ; znak
    ldi r17, 0      ; pozice (0x00-0x0f - prvni radek; 0x40-0x4f - druhy radek)
    call show_char  ; zobraz znak z R16 na pozici z R17

end: jmp end        ; Zastavime program - nekonecna smycka
