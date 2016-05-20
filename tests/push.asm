MOV SS,9
MOV SP,0
MOV AX,42
PUSH AX
;assert(regs[SP]==1)
MOV BX,41
PUSH BX
;assert(regs[SP]==2)
POP CX
;assert(regs[SP]==1)
;assert(regs[CX]==41)
