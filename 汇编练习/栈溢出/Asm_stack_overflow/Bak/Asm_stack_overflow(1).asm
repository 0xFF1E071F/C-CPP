;--------------------
;栈可溢出的源程序
;戚利
;2010.2.28
;--------------------
.386
.model flat, stdcall
option casemap:none

include windows.inc
include user32.inc
include kernel32.inc

includelib user32.lib
includelib kernel32.lib

;数据段
.data
szText db 'HelloWorldPE', 0
;szShellCode db 3 dup(0ffh), 0
szShellCode db 11 dup(0ffh), 0

;代码段
.code

;------------------------
;未检查长度的字符串拷贝函数
;------------------------
_memCopy proc _lpSrc
	local @buf [4]:byte
	
	pushad
	mov esi, _lpSrc
	lea edi, @buf
	mov al, byte ptr[esi] ;取字符串首字节数据给al
	.while al != 0
		mov byte ptr[edi], al  ;写入到edi
		mov al, byte ptr[esi]  ;esi低位入al
		
		inc esi ;源指向下一字节
		inc edi ;目的指向下一字节
	.endw
	popad
	ret
_memCopy endp

start:
	invoke _memCopy, addr szShellCode
	invoke MessageBox,NULL,offset szText,NULL,MB_OK
	invoke ExitProcess,NULL
end start