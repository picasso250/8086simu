# 8086simu
一个比8086CPU还要简单的CPU模拟器

字长：16位

指令长度：16位

### 指令集

| 指令 | 二进制 |
|------|--------|
mov    | 0
load   | 1
save   | 2
add    | 3
sub    | 4
mul    | 5
div    | 6
not    | 7
and    | 8
or     | 9
xor    | A
jmp    | B
jcxz   | C
int    | D
nop    | E

指令构成有两种

	  0             8          16                      32
	  | instruction |    reg    |         idata         |
	  
	  0             8          16          24          32
	  | instruction |   reg 3   |   reg 1   |   reg 2   |

第一种是为含有立即数的指令准备的，第二种只操作寄存器。

所有的立即数都是无符号整数。

指令码共有八位，其中第一位代表此指令是否为立即数指令。

下面是 c 语言式的指令解释

|    指令   | 意义 | 内部编码 |
|-----------|------|----------|
mov  ax, bx | ax = bx | mov   |
mov  ax, 42 | ax = 42 | mov_i |
load ax, [bx] | ax = m[bx] | load |
save [ax], bx | m[ax] = bx | save |
add  ax,bx  | ax += bx  | add |
add  ax, 42 | ax += 42  | add_i
sub  ax, bx | ax -= bx  | sub
sub  ax, 42 | ax -= 42  | sub_i
not  ax     | ax = ~ax  | not
and  ax,bx  | ax &= bx  | and
and  ax,42  | ax &= 42  | and_i
or   ax,bx  | ax \|= bx | or
or   ax,42  | ax \|= 42 | or_i
xor  ax,bx  | ax ^= bx  | xor
xor  ax,42  | ax ^= 42  | xor_i
jmp  ax     | ip = ax   | jmp
jmp  42     | ip = 42   | jmp_i
jcxz ax     | if (cx) jmp(ax); | jcxz
jcxz 42     | if (cx) jmp(42); | jcxz_i
int  ax     | intrpt(ax) | int
int  0      | intrpt(0)  | int_i
nop         | ;          | nop

其中 m[ax] 的意思是寻址 ds:ax ，也就是 `ds * 16 + ax`

