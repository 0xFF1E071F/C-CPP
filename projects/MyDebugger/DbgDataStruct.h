#pragma once

#include <Windows.h>

//DR7与宏定义得出一个值 来判断DR0 DR1 DR2 DR3执行 读或写  写断点
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

//内存断点
#define READ  0
#define WRITE 1

//断点类型
enum tagBpType
{
	//int3断点(bp)
	BP_INT3,
	//硬件断点
	BP_HARD,
	//硬件执行断点
	BP_HARD_EXE,
	//硬件访问
	BP_HARD_ACCESS,
	//硬件写入
	BP_HARD_WRITE,
	//内存断点
	BP_MEM,
	//内存写入断点
	BP_MEM_WRITE,
	//内存读取断点
	BP_MEM_READ,
	//没有断点
	BP_NO,
	//入口断点
	BP_MAIN,
	BGSINGLESTEP,       //步过断点
	BRSINGLESTEP,       //步入断点
	AUTOTRACE,          //自动追踪
};

//保存当前程序运行中的一条代码  主要用于单步和步过
struct StSaveCurCode
{
	char szSrcCode;                     //保存原来的单个字节  CALL设置当前一个字节CC
	char szCurLineCode[4];              //保存当前机器码主要用来分析CALL指令 步过
	UINT nCodeSize;					  //当前代码长度
	DWORD pStartAddress;                //程序入口地址
	DWORD pCurCodeEntryAddress;		  //当前地址，执行完后保存下一条的地址首地址
	char szCurAsmBuf[64];				  //当前程序指令
};

//调试器异常处理标志
struct ExceptionFlag
{
	BOOL bGRunFlag;						//G命令处理标志
	BOOL bSetInFlag;					//单步进入处理标志
	BOOL bSetJumpFlag;					//单步跳过处理标志
	BOOL bHardWareSetinFlag;            //硬件断点引起的单步重设标记
	BOOL bHardWareFlag;                 //硬件标记
	BOOL bSetInt3Flag;                  //int3断点重置标记
	ExceptionFlag()
	{
		bGRunFlag = FALSE;
		bSetInFlag = FALSE;
		bSetJumpFlag = FALSE;
	}
};

//int 3结构
struct tagBpInt3
{
	BOOL  bDelFlag;  //删除标志
	BYTE  btCode;    //原始指令
	DWORD dwAddr;    //断点地址
};

//硬件断点处理标记
struct HareWareFlag
{
	BOOL DR0;        //当前寄存器使用状态
	BOOL DR1;
	BOOL DR2;
	BOOL DR3;
	HareWareFlag()
	{
		DR0 = FALSE; //未使用状态
		DR1 = FALSE;
		DR2 = FALSE;
		DR3 = FALSE;
	}
};

//内存断点(内部详细记录)
struct tagBpMemInfor
{
	BOOL bDelFlag;         //删除标志
	tagBpType pBpType;     //断点类型
	DWORD dwAddr;          //断点地址
	size_t nSize;          //断点长度
	DWORD  OldPageProtect; //原来页的保护属性
	tagBpMemInfor()
	{
		bDelFlag = FALSE;
		dwAddr = 0;
		OldPageProtect = 0;
	}
};

//内存断点(分页用)
struct tagBpMemPage
{
	//预留在这里，
	//xxxxx
	DWORD  OldPageProtect;	 //原来页的保护属性
	//这里还应该有的就是一些分页相关的记录，先不弄了
	std::list<tagBpMemInfor> *m_pBpMemInfor; //内存断点详细记录
};

//内存断点处理标记
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