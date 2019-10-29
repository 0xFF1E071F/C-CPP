#pragma once

#include "CMyUI.h"
#include "CMyFileParse.h"
#include "DbgDataStruct.h"
#include "Decode2Asm.h"
#include "Disasm.h"
#include <list>

struct StSaveCurCode;
struct ExceptionFlag;
struct tagBpInt3;
struct HareWareFlag;

class CMyDbg
{
	//成员函数
public:
	CMyDbg();
	~CMyDbg();
	//文件初始化
	BOOL DbgInit(const char* pszFileName);
	//保存命令
	void SaveCmd(std::string strCmd);
	//打印寄存器
	void DisplayRegisterInfo();

	//命令显示
public:
	//解析处理用户输入的命令
	void HandlerCommand(const char* pszCmd = NULL);
	//显示内存数据
	void DisplayMemData(DWORD dwAddr);
	//显示调试帮助
	void ShowDebugHelp();

	//命令处理
public:
	//dd 显示内存数据
	void DisplayAssembly(char* pCode, DWORD dwCurAddr);
	//u 反汇编
	void ShowCurLineAsm(DWORD dwAddr);
	//r 寄存器
	void ShowContextAsm();
	//e 修改内存数据
	void EditMemData(DWORD dwAddr = 0);
	//t 单步步入
	void StepInProc();
	void DebugStepInProc();//t 单步步入，处理
	//p 单步步过
	void StepJumpProc();
	void DebugStepJumpProc();//p 单步步过，处理
	//g 直接运行或运行到某个地址
	void GRunProc(DWORD dwAddr);

	//调试处理
public:
	//开始调试
	int DbgStartProcess(const char* pszFilePath);
	//分发DEBUG_EVENT事件
	DWORD HandlerEvent();
	//保存寄存器环境
	int GetCPU(CONTEXT& cxt);
	//恢复寄存器环境
	int SetCPU(CONTEXT& cxt);

	//异常处理
	DWORD OnException();
	//处理创建线程
	DWORD OnCreateThread();
	//处理创建进程
	DWORD OnCreateProcess();
	//处理退出线程
	DWORD OnExitThread();
	//处理退出进程
	DWORD OnExitProcess();
	//处理加载DLL
	DWORD OnLoadDll();
	//处理卸载DLL
	DWORD OnUnLoadDll();
	//处理打印调试信息
	DWORD OnOutputDebugString();

	//调试事件处理
public:
	//访问异常处理
	DWORD OnExceptAccess();
	//断点异常处理
	DWORD OnExceptBreakPoint();
	//单步异常处理
	DWORD OnExceptSingleStep();

	//int3软中断处理
public:
	//硬件断点和INT3断点重合处理  迫使INT3断点失效
	void SetBpInt3(DWORD dwAddr);
	void HandlerBpInt3(DWORD dwAddr);
	//设置INT3断点并保存首字节
	BYTE SetBreakPoint(HANDLE hProcess, LPVOID pAddr, BYTE code, BOOL bFlag = TRUE);
	//查找int3断点
	BOOL FindBpInt3(DWORD dwBpAddr, OUT char* pszCode = NULL);
	//删除int3断点
	BOOL DelBpInt3(DWORD dwAddr);
	//显示int3断点列表
	void ShowBpint3();
	//由硬件断点引起的失效INT3断点重设
	BOOL SetBpInt3Again(DWORD dwAddr);

	//硬件断点
public:
	//硬件断点列表
	void ShowBpHard();
	//硬件断点
	void SetBpHard(DWORD SetAddress, int PointType, int nLen = 1);
	//硬件断点设置
	void SetHardWarePoint(DWORD SetAddress, int DR, int PointType, int nLen = 1);
	//硬件DR0
	void SetDR0HardWarePoint(int PointType, int nLen);
	//硬件DR1
	void SetDR1HardWarePoint(int PointType, int nLen);
	//硬件DR2
	void SetDR2HardWarePoint(int PointType, int nLen);
	//硬件DR3
	void SetDR3HardWarePoint(int PointType, int nLen);
	//硬件断点处理事件
	void DebughardWareProc();
	//释放设置的硬件标志
	void DelBpHard(int nChoise);

	//内存断点
public:
	//设置内存断点
	void SetBpMem(DWORD dwAddr, size_t nSize, const char* pszType);
	//显示内存断点列表
	void ShowBpMemList();
	//删除内存断点
	void DelBpMem(size_t nIdx);
	//恢复内存页属性为原始内存页属性
	void SetMemPageAllAccess();
	//设置内存页属性为异常访问的属性
	void SetMemPageNoAccess();

	//解析PE
public:
	//开始解析DLL文件
	void StartDllParse(LOAD_DLL_DEBUG_INFO dllDebugInfo);
	//根据导出表数据，显示相应的DLL和导出函数
	void ShowExportFunc(const char* pszBuf);
	//根据地址查找导出表API函数名称
	BOOL FindFuncNameByAddrInExport(DWORD dwAddr);
	//导入一个脚本文件（后缀名为SCP）。会逐行解析脚本文件的命令，并执行该命令。
	void LoadScript();
	void HandlerLoadScript(const char* pFileMapImage);
	//将用户操作的所有有效命令导出到一个脚本文件
	void ExportScript();
	//查看模块
	void ShowModuleList();

	//成员属性
private:
	CMyUI m_UI;       //UI封装显示    

	PROCESS_INFORMATION m_ProcessInfo;        //被调试程序进程信息
	CREATE_PROCESS_DEBUG_INFO m_DbgProcessInfo;
	DEBUG_EVENT m_Event;                      //被调试程序异常事件 
	HANDLE m_hProcess;                        //被调试程序进程句柄
	HANDLE m_hThread;                         //被调试程序线程句柄

	CONTEXT m_Context;          //寄存器环境
	StSaveCurCode m_CurCode;    //保存当前代码等信息
	BYTE m_oepCode;             //入口地址的临时保存，不加入INT3链表

	DWORD m_URunAddr;           //u指令记录地址	
	BYTE m_GRunCode;            //g命令的单独处理，不加入INT3链表

	DWORD m_uInsCurAddr;        //记录u指令当前地址
	DWORD m_dInsCurAddr;        //记录d指令当前地址
	DWORD m_HWBPointValue;                //保存当前硬件断点的DR7

	std::list<tagBpInt3>::iterator m_BpNode; //保存当前int3地址信息 重置后继续设置CC
	tagBpMemPage m_bpMemPage;   //记录内存断点(分页)相关属性
	DWORD m_bpMemAddr;          //内存断点触发地址(按页)

  //TRUE/FALSE
private:
	BOOL m_bFirstLoadDll;     //DLL加载后是否需要解析的标志，首次加载解析
	BOOL m_IsSystemBreakPoint;//系统断点标志，FALSE不是系统断点
	ExceptionFlag m_EpnFlag;  //各断点异常处理标志
	BOOL bSelIptFlag;         //脚本导入命令标记
	HareWareFlag  m_HWflag;   //硬件断点标记位
	BOOL m_HWHMFlag;          //内存断点与硬件断点组合下断点时存在对应关系
	BOOL bEptScriptFlag;      //判断手动导出，退出进程时，导出命令脚本
	tagBpMemFlag m_bpMemFlag; //内存处理读写标志
	BOOL m_memStepFlag;       //内存断点标志
	BOOL bMemRW;              //内存断点读写类型
	BOOL m_IsInput;           //是否启用输入; TRUE忽略输入,FALSE接收输入
	BOOL m_IptScpFinish;        //导入脚本完成; TRUE完成，FALSE未完成

//链表
private:
	std::list<std::string> m_cmdRecordList; //保存输入的命令
	std::list<std::string> m_cmdIptList;     //导入的命令列表
	std::list<tagBpInt3> m_bpInt3List;      //保存int3断点
	std::list<tagBpMemPage> m_pBpMemList;    //保存内存断点(分页)，内层维护每页的内存断点
	std::list<tagBpMemInfor> m_pBpMemInforList;//保存内存断点用于显示

//PE文件解析
private:
	CMyFileParse m_hFileInfor; //文件PE解析
};

