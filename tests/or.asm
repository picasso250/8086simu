MOV AX,3
OR  AX,1
;assert(regs[AX] == 3)
MOV AX,3
MOV BX,2
OR  AX,BX
;assert(regs[AX] == 3)
