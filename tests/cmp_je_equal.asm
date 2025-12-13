; tests/cmp_je_equal.asm
MOV AX, 1
MOV BX, 5
MOV CX, 5
CMP CX, BX ; ZF should be set
JE 4       ; Jump over next instruction
MOV AX, 10 ; This should be skipped
ADD AX, 1  ; AX becomes 2
;assert(regs[AX] == 2)
INT 0      ; Halt
