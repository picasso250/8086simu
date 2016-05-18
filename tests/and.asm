MOV AX,3
AND AX,1
;assert(regs[AX] == 1)
MOV AX,3
MOV BX,2
AND AX,BX
;assert(regs[AX] == 2)
