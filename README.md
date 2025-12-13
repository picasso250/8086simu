# 8086simu
一个比8086CPU还要简单的CPU模拟器

字长：16位

指令长度：16位

### 指令集

| 指令 | 二进制码(十六进制表示) |
|------|--------|
nop    | 0
mov    | 1
load   | 2
save   | 3
add    | 4
sub    | 5
mul    | 6
div    | 7
not    | 8
and    | 9
or     | A
xor    | B
jmp    | C
jcnxz   | D
int    | E
push   | F
pop    | 10
cmp    | 11
je     | 12
jne    | 13
jg     | 14
jge    | 15
jl     | 16
jle    | 17

### 标志寄存器 (PSW)

`PSW` 是一个16位的程序状态字寄存器，它包含着CPU执行指令后产生的各种状态信息，主要用于条件跳转。

- **ZF (Zero Flag)**: 零标志位。如果算术或逻辑运算的结果为0，则ZF被置位（为1），否则清零（为0）。
- **SF (Sign Flag)**: 符号标志位。如果算术运算的结果为负（即最高位为1），则SF被置位，否则清零。
- **OF (Overflow Flag)**: 溢出标志位。如果带符号算术运算的结果超出了16位有符号数的表示范围，则OF被置位。

`ADD`, `SUB`, `CMP` 等指令会影响这些标志位。

指令构成有两种

	  0             8          16                      32
	  | instruction |    reg    |         idata         |
	  
      0             8          16          24          32
      | instruction |   reg 3   |   reg 1   |   reg 2   |

	  0             8          16
	  | instruction |    reg    |

第一种是为含有立即数的指令准备的，第二种只操作寄存器。

第三种只有一个寄存器，不过为了实现简单，我们不准备使用这种指令。

所有的立即数都是无符号整数。

指令码共有八位，其中第一位代表此指令是否为立即数指令。

下面是 c 语言式的指令解释

|    指令   | 意义 | 内部编码 |
|-----------|------|----------|
nop         | ;          | nop
mov  ax, bx | ax = bx | mov   |
mov  ax, 42 | ax = 42 | mov_i |
load ax, [bx] | ax = m[bx] | load |
save [ax], bx | m[ax] = bx | save |
add  ax,bx  | ax += bx  | add |
add  ax, 42 | ax += 42  | add_i
sub  ax, bx | ax -= bx  | sub
sub  ax, 42 | ax -= 42  | sub_i
cmp  ax, bx | ax - bx, set flags | cmp
cmp  ax, 42 | ax - 42, set flags | cmp_i
not  ax     | ax = ~ax  | not
and  ax,bx  | ax &= bx  | and
and  ax,42  | ax &= 42  | and_i
or   ax,bx  | ax |= bx | or
or   ax,42  | ax |= 42 | or_i
xor  ax,bx  | ax ^= bx  | xor
xor  ax,42  | ax ^= 42 | xor_i
jmp  ax     | ip = ax   | jmp
jmp  42     | ip = 42   | jmp_i
jcnxz ax     | if (cx) jmp(ax); | jcnxz
jcnxz 42     | if (cx) jmp(42); | jcxz_i
je   42     | if (ZF==1) jmp(42) | je_i
jne  42     | if (ZF==0) jmp(42) | jne_i
jg   42     | if (ZF==0 and SF==OF) jmp(42) | jg_i
jge  42     | if (SF==OF) jmp(42) | jge_i
jl   42     | if (SF!=OF) jmp(42) | jl_i
jle  42     | if (ZF==1 or SF!=OF) jmp(42) | jle_i
int  ax     | intrpt(ax) | int
int  0      | intrpt(0)  | int_i
push ax     | s[--sp] = ax | 
pop  ax     | ax = s[sp++] | 

其中 m[ax] 的意思是寻址 ds:ax ，也就是 `ds * 16 + ax`

PC 和 SP 都是以16位为字长的（8086是以8位为字长）

jmp 和 jcnxz 都是 short jmp
