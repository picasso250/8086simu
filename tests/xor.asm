MOV AX,3
XOR AX,1
;assert(regs[AX] == 2)
MOV AX,3
MOV BX,2
XOR AX,BX
;assert(regs[AX] == 1)
