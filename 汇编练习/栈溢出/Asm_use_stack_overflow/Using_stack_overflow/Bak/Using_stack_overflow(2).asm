;----------------------
;����ջ���ִ�еڶ�������
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
;szShellCode dd 0ffffffffh, 11111111h, 040103Ch, 0 ;����д00�ᱻɾ�������40103C����
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

invoke MessageBox,NULL,offset szText2,NULL,MB_OK ;ִֻ���������

invoke ExitProcess,NULL

end start

;-----------------------------------
;����_memCopyû�жԿ���������������һջ���©����
;�ó���ִ��_memCopy������ִ�����ǵĴ���
;���磬������_memCopy��ֱ��ִ�еڶ�������
;�Ǿ���Ҫ��취��_memCopyִ�к��eipָ��ڶ������ں�����ַ
;�����������shellcode����ջ����������ʵ�ֿ���eip
;
;ջ�ṹ      �͵�ַ
;    pushad        |-----8
;                  |-----7
;                  |-----6
;                  |-----5
;                  |-----4
;                  |-----3   [...]
;                  |-----2   [ebp-C]
;                  |-----1   [ebp-8]
;
;	local          |-----buf [ebp-4] ;��������λ��
;       
;	ebp
;                  |-----ebp
;
;	ret			   |-----ret [ebp+4] ;��ʼ����λ�� EIP
;            �ߵ�ַ
;
;
;����ShellCode
;��ջ˳��ret->ebp->local��һ����12���ֽڵĲ����ռ�
;szShellCode db 0xffffffffh, @2, @3,0
;�����ǳ���@2��@3,��Ҫ��������Ϻ�ret��ַ���պ�Ϊ�ڶ������ڵĵ�ַ���ɡ�
;�������鿴�ڶ������ں�����ַ0x0040103C��
;���ShellCode 0xffffffffh, 11111111h, 040103Ch, 0
;
;------------------------------------