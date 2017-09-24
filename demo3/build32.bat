@echo off
gcc -Iinc -I../lightning/inc -D__WORDSIZE=32 -DHAVE_CONFIG_H=1 ../lightning/src/jit_disasm.c ../lightning/src/jit_memory.c ../lightning/src/jit_note.c ../lightning/src/jit_print.c ../lightning/src/lightning.c ../lightning/src/mman.c src/*.c -lgw32c -o cgl.exe
pause