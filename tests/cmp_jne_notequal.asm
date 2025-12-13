; tests/cmp_jne_notequal.asm
MOV AX, 1
MOV BX, 5
MOV CX, 6
CMP CX, BX ; ZF should be clear
JNE 4      ; Jump should be taken
MOV AX, 10 ; This should be skipped
ADD AX, 1  ; AX becomes 2
;assert(regs[AX] == 2)
INT 0      ; Halt
