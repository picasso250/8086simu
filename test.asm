MOV AX,IP
MOV DS,CS
MOV DX,AX
MOV CX,11
ADD DX,CX
; LOOP
MOV  BX,[AX]
MOV  [DX],BX
INC  AX
INC  DX
SUB  CX,1 ; TODO
JCXZ -4
