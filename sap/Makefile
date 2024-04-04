# 
# 	Copyright 2008-2011 Michel Pollet <buserror@gmail.com>
#
#	This file is part of simavr.
#
#	simavr is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	simavr is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with simavr.  If not, see <http://www.gnu.org/licenses/>.

#   Modified by EETagent

target=	emulator
asm_program= program

simavr = ../

IPATH = .
IPATH += ./parts
IPATH += ${simavr}/include
IPATH += ${simavr}/simavr/sim

VPATH = .
VPATH += ./parts

LDFLAGS += -lpthread

include ./Makefile.opengl

all: obj ${target} 

asm: ${asm_program}.hex

run: ${target} asm
	./${OBJ}/${target}.elf ${asm_program}.hex

include ${simavr}/Makefile.common

board = ${OBJ}/${target}.elf

${board} : ${OBJ}/hd44780.o
${board} : ${OBJ}/hd44780_glut.o
${board} : ${OBJ}/${target}.o

${target}: ${board}
	@echo $@ done

${asm_program}.hex: ${asm_program}.asm asm/*.inc
	avra ${asm_program}.asm

clean: clean-${OBJ}
	rm -rf *.hex *.obj *.a *.axf ${target} *.vcd .*.swo .*.swp .*.swm .*.swn