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
j dd 0 ;����

.code
start:
	;�Ա�������ݽ����޸�
	.while j < 4
		lea eax, szBuffer ;ȡ��szBuffer����Ч��ַ��ָ���׵�ַ
		add eax, j        ;szBuffer[j]
		mov bl, byte ptr[eax] ; ,byte ptr[eax] �൱�ڶ�szBuffer[j]��
		inc bl                ;szBuffer[j]��ֵ��1
		mov byte ptr[eax], bl ; byte ptr[eax] �൱�Զ�szBuffer[j]д
		
		inc j ;����
	.endw
	
	invoke MessageBox, NULL, offset szBuffer, offset szCaption, MB_OK
	invoke ExitProcess, NULL	
end start