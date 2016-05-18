MOV AX,9
DIV AX,4
;assert(regs[AX] == 2)
;assert(regs[DX] == 1)
MOV AX,11
MOV BX,7
DIV AX,BX
;assert(regs[AX] == 1)
;assert(regs[DX] == 4)
