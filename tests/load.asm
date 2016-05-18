MOV AX,42
MOV BX,0
SAVE [BX],AX
LOAD CX,[BX]
;assert(regs[CX]==42)
MOV AX,41
MOV BX,1
SAVE [BX],AX
LOAD DX,[BX]
;assert(regs[DX]==41)
