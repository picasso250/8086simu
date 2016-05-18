MOV AX,42
MOV BX,0
SAVE [BX],AX
;assert(load(0)==42)
MOV AX,41
MOV BX,1
SAVE [BX],AX
;assert(load(1)==41)
