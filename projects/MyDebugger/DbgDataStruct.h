#pragma once

#include <Windows.h>

//DR7��궨��ó�һ��ֵ ���ж�DR0 DR1 DR2 DR3ִ�� ����д  д�ϵ�
//DR0
#define ONEDR0EVALUE 	0X301
#define ONEDR0WVALUE	0x10701
#define ONEDR0RWVALUE	0x30701

#define TWODR0EVALUE 	0X301
#define TWODR0WVALUE	0x50701
#define TWODR0RWVALUE	0x70701

#define FOURDR0EVALUE 	0X301
#define FOURDR0WVALUE	0x0D0701
#define FOURDR0RWVALUE	0x0F0701

//DR1
#define ONEDR1EVALUE 	0x304
#define ONEDR1WVALUE	0x100704
#define ONEDR1RWVALUE	0x300704

#define TWODR1EVALUE 	0x304
#define TWODR1WVALUE	0x500704
#define TWODR1RWVALUE	0x700704

#define FOURDR1EVALUE 	0x304
#define FOURDR1WVALUE	0x0D00704
#define FOURDR1RWVALUE	0x0F00704


//DR2
#define ONEDR2EVALUE 	0x310
#define ONEDR2WVALUE	0x1000710
#define ONEDR2RWVALUE	0x3000710

#define TWODR2EVALUE 	0x301
#define TWODR2WVALUE	0x5000710
#define TWODR2RWVALUE	0x7000710

#define FOURDR2EVALUE 	0x301
#define FOURDR2WVALUE	0x0D000710
#define FOURDR2RWVALUE	0x0F000710

//DR3
#define ONEDR3EVALUE 	0x340
#define ONEDR3WVALUE	0x10000740
#define ONEDR3RWVALUE	0x30000740

#define TWODR3EVALUE 	0x340
#define TWODR3WVALUE	0x50000740
#define TWODR3RWVALUE	0x70000740

#define FOURDR3EVALUE 	0x340
#define FOURDR3WVALUE	0x0D0000740
#define FOURDR3RWVALUE	0x0F0000740

//�ڴ�ϵ�
#define READ  0
#define WRITE 1

//�ϵ�����
enum tagBpType
{
	//int3�ϵ�(bp)
	BP_INT3,
	//Ӳ���ϵ�
	BP_HARD,
	//Ӳ��ִ�жϵ�
	BP_HARD_EXE,
	//Ӳ������
	BP_HARD_ACCESS,
	//Ӳ��д��
	BP_HARD_WRITE,
	//�ڴ�ϵ�
	BP_MEM,
	//�ڴ�д��ϵ�
	BP_MEM_WRITE,
	//�ڴ��ȡ�ϵ�
	BP_MEM_READ,
	//û�жϵ�
	BP_NO,
	//��ڶϵ�
	BP_MAIN,
	BGSINGLESTEP,       //�����ϵ�
	BRSINGLESTEP,       //����ϵ�
	AUTOTRACE,          //�Զ�׷��
};

//���浱ǰ���������е�һ������  ��Ҫ���ڵ����Ͳ���
struct StSaveCurCode
{
	char szSrcCode;                     //����ԭ���ĵ����ֽ�  CALL���õ�ǰһ���ֽ�CC
	char szCurLineCode[4];              //���浱ǰ��������Ҫ��������CALLָ�� ����
	UINT nCodeSize;					  //��ǰ���볤��
	DWORD pStartAddress;                //������ڵ�ַ
	DWORD pCurCodeEntryAddress;		  //��ǰ��ַ��ִ����󱣴���һ���ĵ�ַ�׵�ַ
	char szCurAsmBuf[64];				  //��ǰ����ָ��
};

//�������쳣�����־
struct ExceptionFlag
{
	BOOL bGRunFlag;						//G������־
	BOOL bSetInFlag;					//�������봦���־
	BOOL bSetJumpFlag;					//�������������־
	BOOL bHardWareSetinFlag;            //Ӳ���ϵ�����ĵ���������
	BOOL bHardWareFlag;                 //Ӳ�����
	BOOL bSetInt3Flag;                  //int3�ϵ����ñ��
	ExceptionFlag()
	{
		bGRunFlag = FALSE;
		bSetInFlag = FALSE;
		bSetJumpFlag = FALSE;
	}
};

//int 3�ṹ
struct tagBpInt3
{
	BOOL  bDelFlag;  //ɾ����־
	BYTE  btCode;    //ԭʼָ��
	DWORD dwAddr;    //�ϵ��ַ
};

//Ӳ���ϵ㴦����
struct HareWareFlag
{
	BOOL DR0;        //��ǰ�Ĵ���ʹ��״̬
	BOOL DR1;
	BOOL DR2;
	BOOL DR3;
	HareWareFlag()
	{
		DR0 = FALSE; //δʹ��״̬
		DR1 = FALSE;
		DR2 = FALSE;
		DR3 = FALSE;
	}
};

//�ڴ�ϵ�(�ڲ���ϸ��¼)
struct tagBpMemInfor
{
	BOOL bDelFlag;         //ɾ����־
	tagBpType pBpType;     //�ϵ�����
	DWORD dwAddr;          //�ϵ��ַ
	size_t nSize;          //�ϵ㳤��
	DWORD  OldPageProtect; //ԭ��ҳ�ı�������
	tagBpMemInfor()
	{
		bDelFlag = FALSE;
		dwAddr = 0;
		OldPageProtect = 0;
	}
};

//�ڴ�ϵ�(��ҳ��)
struct tagBpMemPage
{
	//Ԥ�������
	//xxxxx
	DWORD  OldPageProtect;	 //ԭ��ҳ�ı�������
	//���ﻹӦ���еľ���һЩ��ҳ��صļ�¼���Ȳ�Ū��
	std::list<tagBpMemInfor> *m_pBpMemInfor; //�ڴ�ϵ���ϸ��¼
};

//�ڴ�ϵ㴦����
struct tagBpMemFlag
{
	BOOL bMemRead;
	BOOL bMemWrite;
	tagBpMemFlag()
	{
		bMemRead = FALSE;
		bMemWrite = FALSE;
	}

};