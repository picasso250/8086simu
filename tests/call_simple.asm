;int foo(int a, int b) {
;	int c = 3;
;	int d = 2;
;	return a + b + c + d;
;}
;int main() {
;	int a = 2;
;	int b;
;	int c;
;	b = foo(4, 5); // 14
;	c = a * b; // 28
;}
MOV SS,99
MOV DS,SS
;int a = 3
MOV SP,99
MOV AX,2
PUSH AX
;assert(load(regs[SS]*16+regs[SP])==2)
;int b
MOV BX,0
PUSH BX
;int c
MOV CX,0
PUSH CX
;foo(4, 5)
;prepare params
MOV AX,4
PUSH AX
MOV BX,5
PUSH BX
;assert(regs[SP]==99-5)
;foo()
;CALL foo
; store PC
PUSH IP
JMP 46
; back 2 params
ADD SP,2
MOV BX,SP
ADD BX,1
;assert(regs[AX]==14)
;assert(regs[BX]==99-2)
; return ; BX ; b = foo(4,5)
SAVE [BX],AX
; a * b
MOV CX,SP
ADD CX,2
;assert(regs[CX]==99-1)
LOAD AX,[CX]
;assert(regs[AX]==2)
MOV DX,SP
ADD DX,1
;assert(regs[DX]==99-2)
LOAD BX,[DX]
;assert(regs[AX]==2)
;assert(regs[BX]==14)
MUL AX,BX
SAVE [SP],AX
;assert(load(regs[SS]*16+regs[SP])==28)
INT 0

;foo:
;int c = 3
MOV CX,3
PUSH CX
;int d = 2
MOV DX,2
PUSH DX
;assert(regs[SP]==99-8)
MOV CX,SP
ADD CX,4
LOAD AX,[CX]
;assert(regs[AX]==4)
SUB CX,1
LOAD BX,[CX]
;assert(regs[BX]==5)
ADD AX,BX
MOV CX,SP
ADD CX,1
LOAD BX,[CX]
;assert(regs[BX]==3)
ADD AX,BX
LOAD BX,[SP]
;assert(regs[BX]==2)
ADD AX,BX
;assert(regs[AX]==14)
;RET
ADD SP,2
POP DX
ADD DX,4
JMP DX
