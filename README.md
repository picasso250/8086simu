# 8086simu
一个比8086CPU还要简单的CPU模拟器

字长：16位

指令长度：16位

### 指令集

| 指令 | 二进制 |
|------|--------|
mov   | 0
load  | 1
save  | 2
add   | 3
sub   | 4
not   | 5
and   | 6
or    | 7
jmp   | 8
jcxz  | 9
int   | A
nop   | B

指令构成有两种

	  0             4         8                      16
	  | instruction |   reg   |         idata         |
	  
	  0                       8          12          16
	  |      instruction      |   reg 1   |   reg 2   |

称第一种为短指令，第二种为长指令

具有长指令和短指令的指令如下：

| 指令 | 意义 |
|------|------|
mov  ax, bx | ax = bx
mov  ax, 42 | ax = 42
load ax, [bx] | ax = m[bx]
save [ax], bx | m[ax] = bx
add  ax,bx  | ax += bx
add  ax, 42 | ax += 42
sub  ax, bx | ax -= bx
sub  ax, 42 | ax -= 42
not  ax     | ax = ^ax
and  ax,bx  | ax &= bx
or   ax,bx  | ax \|= bx
jmp  ax     | ip = ax
jmp  42     | ip += 42
jcxz ax     | if (cx) jmp(ax);
jcxz 42     | if (cx) jmp(42);

其中 m[ax] 的意思是寻址 dx:ax ，也就是 `dx * 16 + ax`

凡是涉及到 `idata` 的，指令都是长指令，指令前缀都是 `0xF`
