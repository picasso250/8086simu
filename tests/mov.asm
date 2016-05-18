MOV AX,42
;assert(regs[AX]==42)
MOV AX,65535
;assert(regs[AX]==65535)
MOV AX,0
;assert(regs[AX]==0)
MOV AX,42
MOV BX,AX
;assert(regs[AX]==42)
