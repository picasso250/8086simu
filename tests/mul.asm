MOV AX,3
MUL AX,4
;assert(regs[AX] == 12)
MOV AX,3
MOV BX,2
MUL AX,BX
;assert(regs[AX] == 6)
