.386
.model flat, stdcall
option casemap:none

include windows.inc
include user32.inc
include kernel32.inc

includelib user32.lib
includelib kernel32.lib

.data
szBuffer db "test", 0  ;uftu
szCaption db "abcd", 0
j dd 0 ;遍历

.code
start:
	;对标题和内容进行修改
	.while j < 4
		lea eax, szBuffer ;取得szBuffer的有效地址，指向首地址
		add eax, j        ;szBuffer[j]
		mov bl, byte ptr[eax] ; ,byte ptr[eax] 相当于对szBuffer[j]读
		inc bl                ;szBuffer[j]的值加1
		mov byte ptr[eax], bl ; byte ptr[eax] 相当对对szBuffer[j]写
		
		inc j ;遍历
	.endw
	
	invoke MessageBox, NULL, offset szBuffer, offset szCaption, MB_OK
	invoke ExitProcess, NULL	
end start