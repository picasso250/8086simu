; 3 > 5, should NOT jump
MOV AX, 1
MOV BX, 5
MOV CX, 3
CMP CX, BX ; 3 < 5. res = 3 - 5 = -2. ZF=0, SF=1, OF=0. SF!=OF. No Jump.
JG 4
MOV AX, 10 ; AX becomes 10
ADD AX, 1  ; AX becomes 11
;assert(regs[AX] == 11)
INT 0
