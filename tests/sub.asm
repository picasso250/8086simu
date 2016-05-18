MOV AX,3
SUB AX,1
;assert(regs[AX] == 2)
MOV AX,3
MOV BX,2
SUB AX,BX
;assert(regs[AX] == 1)
