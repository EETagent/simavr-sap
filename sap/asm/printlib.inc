; Library for Arduino LCD 1602 keypad shield
;
; for BI-SAP at FIT, CTU
;
; Shield pinout:
; UNO   AVR	    LCD
; D4	PD4	    D4
; D5	PD5	    D5
; D6	PD6	    D6
; D7	PD7	    D7
; D8	PB0	    RS
; D9	PB1	    E
; D10	PB2	    Backlight
; A0	ADC0	    Buttons
    
; lcd_init: setup IO ports and initialize the LCD
init_disp:
    push    r20
    push    r24
    push    r25
    push    r16

    ; set IO ports
    cli
    in	    r20, PORTB
    andi    r20, 0xfc
    out	    PORTB, r20

    in	    r20, DDRB
    ori	    r20, 0x03	; set RS, E, backlight to output
    out	    DDRB, r20

    in	    r20, PORTD
    andi    r20, 0x0f
    out	    PORTD, r20

    in	    r20, DDRD
    ori	    r20, 0xf0   ; set PD(7) - PD(4) to outputs
    out	    DDRD, r20
    sei

    ldi	    r25, 0
    ldi	    r24, 100
    call    lcd_delay_us
; pokus
    ldi	    r25, 65
    ldi	    r24, 255
    call    lcd_delay_us

cli
    in	    r20, PORTD
    andi    r20, 0x0f
    ori	    r20, 0x30	    ; cmd: function set: 4-bit
    out	    PORTD, r20
    sei
    call    lcd_clock_cmd   ; only “half” command is sent


    ldi	    r25, 17
    ldi	    r24, 255
    call    lcd_delay_us


cli
    in	    r20, PORTD
    andi    r20, 0x0f
    ori	    r20, 0x30	    ; cmd: function set: 4-bit
    out	    PORTD, r20
    sei
    call    lcd_clock_cmd   ; only “half” command is sent


    ldi	    r25, 0
    ldi	    r24, 120
    call    lcd_delay_us

cli
    in	    r20, PORTD
    andi    r20, 0x0f
    ori	    r20, 0x30	    ; cmd: function set: 4-bit
    out	    PORTD, r20
    sei
    call    lcd_clock_cmd   ; only “half” command is sent



; pokus end

    ; init display
    cli
    in	    r20, PORTD
    andi    r20, 0x0f
    ori	    r20, 0x20	    ; cmd: function set: 4-bit
    out	    PORTD, r20
    sei
    call    lcd_clock_cmd   ; only “half” command is sent

    ldi	    r16, 0x28	    ; 4-bit, 2 lines, 8×5 font
    call    lcd_send_cmd

    ldi	    r16, 0x0c	    ; cursor off
    call    lcd_send_cmd

    ldi	    r16, 0x06	    ; increment mode
    call    lcd_send_cmd

    call    lcd_clear

    pop	    r16
    pop	    r25
    pop     r24
    pop     r20
    ret

; show_char: show character in r16 at position in r17
; position format: r17 = ---rcccc
; r ... row (0..1)
; c ... column (0..15)
show_char:
    ; cursor
    call lcd_set_cursor
    ; show
    call lcd_show_char
    ret

; lcd_set_cursor: set cursor to position in r17:
; r17 = ---rcccc (row, column)
lcd_set_cursor:
    push r16
    mov r16, r17
    call lcd_set_ddram_addr
    pop r16
    ret

; lcd_set_cgram_addr: set cgram address (r16)
lcd_set_cgram_addr:
    push r24
    push r25
    push r16
    andi r16, 0x3F
    ori r16, 0x40
    call lcd_send_cmd
    ldi r25, 0
    ldi r24, 40
    call lcd_delay_us
    pop r16
    pop r25
    pop r24
    ret
    
; lcd_set_ddram_addr: set ddram address (r16)
lcd_set_ddram_addr:
    push r16
    ori r16, 0x80
    call lcd_send_cmd
    ;ldi r25, 0
    ;ldi r24, 40
    ;call lcd_delay_us
    pop r16
    ret
    
; lcd_clock_cmd: start lcd command/write
; delay for 40 μs
lcd_clock_cmd:
    push    r24
    push    r25
    sbi	    PORTB, 1
    ldi	    r25, 0
    ldi	    r24, 20
    call    lcd_delay_us
    cbi	    PORTB, 1
    ldi	    r24, 20
    call    lcd_delay_us
    pop	    r25
    pop	    r24
    ret
    
; lcd_send_cmd: send command (r16) to lcd
lcd_send_cmd:
    push    r16
    push    r17
    push    r20

    mov	    r17, r16
    andi    r17, 0xf0
    lsl	    r16
    lsl	    r16
    lsl	    r16
    lsl	    r16
    cbi	    PORTB, 0

    cli
    in	    r20, PORTD
    andi    r20, 0x0f
    or	    r20, r17
    out	    PORTD, r20
    sei
    call    lcd_clock_cmd

    cli
    in	    r20, PORTD
    andi    r20, 0x0f
    or	    r20, r16
    out	    PORTD, r20
    sei
    call    lcd_clock_cmd

    pop	    r20
    pop	    r17
    pop     r16
    ret

; lcd_send_data: send data (r16) to lcd
lcd_send_data:
    push    r16
    push    r17
    push    r20

    mov	    r17, r16
    andi    r17, 0xf0
    lsl	    r16
    lsl	    r16
    lsl	    r16
    lsl	    r16
    sbi	    PORTB, 0

    cli
    in	    r20, PORTD
    andi    r20, 0x0f
    or	    r20, r17
    out	    PORTD, r20
    sei
    call    lcd_clock_cmd

    cli
    in	    r20, PORTD
    andi    r20, 0x0f
    or	    r20, r16
    out	    PORTD, r20
    sei
    call    lcd_clock_cmd
    cbi	    PORTB, 0

    pop	    r20
    pop	    r17
    pop	    r16
    ret

lcd_show_char:
    rjmp lcd_send_data
    ;call lcd_send_data
    ;ret

lcd_clear:
    push    r25
    push    r24
    push    r16
    ldi	    r16, 0x01
    call    lcd_send_cmd
    ldi	    r25, 7
    ldi	    r24, 255
    call    lcd_delay_us
    pop	    r16
    pop	    r24
    pop	    r25
    ret

; lcd_light_on: turn on LCD backlight
; LCD backlight is on Arduino pin D10
; that translates to PORTB(2) for UNO
lcd_light_on:
    sbi	    PORTB, 2
    ret

; lcd_light_off: turn off LCD backlight
lcd_light_off:
    cbi	    PORTB, 2
    ret

; lcd_delay_us: wait (r25:r24) μs
lcd_delay_us:
    push    r20
lcd_delay_us_l0:
    ldi	    r20, 4
lcd_delay_us_l1:
    dec	    r20
    brne    lcd_delay_us_l1
    sbiw    r24, 1
    brne    lcd_delay_us_l0
    pop	    r20
    ret
