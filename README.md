simavr_sap_emulator - Atmel AVR atmega328p simulator for FIT SAP LCD 1602 keypad board
======

## Compiling ASM code using avra (avrasm2 is also supported)

```sh
avra program.asm
```

## Running the simulator

```sh
simavr_sap_emulator program.hex
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

Check README.simavr.md for more info on the simulator
