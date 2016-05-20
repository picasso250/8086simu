MOV AX,0
JMP 4
ADD AX,1
ADD AX,1
;assert(regs[AX]==1)

MOV BX,IP
ADD BX,8
JMP BX
ADD AX,2
ADD AX,3
;assert(regs[AX]==4)
