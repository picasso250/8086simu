; 5 < 3, should NOT jump
MOV AX, 1
MOV BX, 3
MOV CX, 5
CMP CX, BX ; 5 > 3. res = 5 - 3 = 2. ZF=0, SF=0, OF=0. SF=OF. No Jump.
JL 4
MOV AX, 10 ; AX becomes 10
ADD AX, 1  ; AX becomes 11
;assert(regs[AX] == 11)
INT 0
