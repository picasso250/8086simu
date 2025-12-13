; 3 < 5, should jump
MOV AX, 1
MOV BX, 5
MOV CX, 3
CMP CX, BX ; 3 < 5. res = 3 - 5 = -2. ZF=0, SF=1, OF=0. SF!=OF. Jump.
JL 4
MOV AX, 10 ; skip
ADD AX, 1  ; AX becomes 2
;assert(regs[AX] == 2)
INT 0
