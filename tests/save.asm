MOV DS,9
MOV AX,42
MOV BX,0
SAVE [BX],AX
;assert(load(9*0x10+0)==42)
MOV AX,41
MOV BX,1
SAVE [BX],AX
;assert(load(9*0x10+1)==41)
