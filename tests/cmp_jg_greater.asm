; 5 > 3, should jump
MOV AX, 1
MOV BX, 3
MOV CX, 5
CMP CX, BX ; 5 > 3. ZF=0, SF=0, OF=0. SF=OF. Jump.
JG 4
MOV AX, 10 ; skip
ADD AX, 1  ; AX becomes 2
;assert(regs[AX] == 2)
INT 0
