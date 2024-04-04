simavr_sap_emulator - Atmel AVR atmega328p simulator for FIT SAP LCD 1602 keypad board
======

<img width="631" alt="image" src="https://github.com/EETagent/simavr-sap-experiment/assets/20557318/ef75870d-84ce-4088-942b-073e04b58874">

## Compiling ASM code using avra (avrasm2 is also supported)

```sh
avra program.asm
```

### Example program

Please only define .org 0 and always include pritnlib.inc after

```asm
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
```

## Running the simulator

```sh
simavr_sap_emulator program.hex
```

## VSCode bindings

.vscode/tasks.json

CMD + SHIFT + B now builds active .asm file


```json
{
    "version": "2.0.0",
    "tasks": [
      {
        "label": "Assembler",
        "type": "shell",
        "command": "avra ${file}",
        "problemMatcher": {
          "base": "$gcc",
          "fileLocation": [
            "relative",
          ]
        },
        "group": {
          "kind": "build",
          "isDefault": true
        }
      },
      {
        "label": "Emulate",
        "type": "shell",
        "command": "simavr_sap_emulator ${fileDirname}/${fileBasenameNoExtension}.hex",
        "problemMatcher": [],
        "group": {
          "kind": "test",
          "isDefault": true
        }
      }
    ]
  }
```

## GDB

```sh
simavr_sap_emulator program.hex -d
```

```sh
avr-gdb -q -n -ex 'target remote 127.0.0.1:1234'
```

### Printing registers

```gdb
(gdb) info registers
```

## macOS

### Building

```sh
brew tap osx-cross/avr
brew install avr-gcc@12
```

```sh
make
```

### Testing

```sh
cd sap
make
make run

```

### Tools

you can use the included static programs in the zip

```sh
brew install avrdude avra
```

## Linux

### Building

```sh
sudo apt-get install gcc-avr avr-libc
```

```sh
make
```

### Testing

```sh
cd sap
make
make run
```

### Tools

```sh
sudo apt-get install avrdude avra
```

## More Info

Check [README.simavr.md](https://github.com/EETagent/simavr-sap-experiment/blob/master/README.simavr.md) for more info on the simulator
