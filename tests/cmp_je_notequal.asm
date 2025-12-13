; tests/cmp_je_notequal.asm
MOV AX, 1
MOV BX, 5
MOV CX, 6
CMP CX, BX ; ZF should be clear
JE 4       ; Jump should NOT be taken
MOV AX, 10 ; This should be executed. AX becomes 10
ADD AX, 1  ; AX becomes 11
;assert(regs[AX] == 11)
INT 0      ; Halt
