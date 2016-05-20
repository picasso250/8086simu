MOV SS,9
MOV SP,10
MOV AX,42
PUSH AX
;assert(regs[SP]==9)
MOV BX,41
PUSH BX
;assert(regs[SP]==8)
POP CX
;assert(regs[SP]==9)
;assert(regs[CX]==41)
