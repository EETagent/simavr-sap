/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * General stdiodemo defines
 *
 * $Id: defines.h 2186 2010-09-22 10:25:15Z aboyapati $
 */

// ; Shield pinout:
// ; UNO   AVR	    LCD
// ; D4	PD4	    D4
// ; D5	PD5	    D5
// ; D6	PD6	    D6
// ; D7	PD7	    D7
// ; D8	PB0	    RS
// ; D9	PB1	    E
// ; D10	PB2	    Backlight
// ; A0	ADC0	    Buttons

/* HD44780 LCD port connections */
#define HD44780_RS B, 0
#define HD44780_E B, 1

/* The data bits have to be not only in ascending order but also consecutive. */
#define HD44780_D4 D, 4