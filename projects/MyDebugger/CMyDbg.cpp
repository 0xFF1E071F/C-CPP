#include "CMyDbg.h"
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <iostream>

//反汇编
#pragma comment(lib,"MyDisAsm.lib")

//INT3断点，使用0xcc下断
const BYTE _0xCC = 0xCC;

/*
标志寄存器
11	10	9	  8	  7	  6	  5	  4	  3	  2	  1	  0
OF	DF	IF	TF	SF	ZF	  	AF	  	PF	  	CF

	  1置位     0复位
OF    OV        NV    溢出
DF    DN        UP    方向
IF    EI        DI    中断
SF    NG        PL    符号
ZF    ZR        NZ    零
AF    AC        NA    辅助进位
PF    PE        PO    奇偶
CF    CY        NC    进位
*/
#define  _FLAG_OF 0x800
#define  _FLAG_IF 0x200
#define  _FLAG_TF 0x100
#define  _FLAG_SF 0x80
#define  _FLAG_ZF 0x40
#define  _FLAG_AF 0x10
#define  _FLAG_PF 0x4
#define  _FLAG_CF 0x1

CMyDbg::CMyDbg()
{
	m_IsSystemBreakPoint = FALSE;
	m_bFirstLoadDll = TRUE;
	m_HWBPointValue = 0;
	m_IsInput = FALSE;
	m_IptScpFinish = FALSE;
}

CMyDbg::~CMyDbg()
{
	m_IsInput = FALSE;
	m_IptScpFinish = FALSE;
}

/*
功能：文件初始化，主要用于检测是否是正常的PE文件，
对于正常的PE文件，映射到内存中，便于后边对PE相关分析
参数：
const char* szFile：文件路径+文件名
*/
BOOL CMyDbg::DbgInit(const char* pszFileName)
{
	BOOL ret = m_hFileInfor.ParseFile(pszFileName);
	if (!ret)
	{
		return FALSE;
	}

	m_UI.Output("File OK\n");
	return TRUE;
}

/*
显示当前行汇编
显示格式：地址   机器码   汇编指令
*/
void CMyDbg::DisplayAssembly(char* pszCode, DWORD dwCurAddr)
{
	char szCurAsmBuf[256] = { 0 }; //汇编指令
	UINT nSize = 0;                //指令长度

	//反汇编
	Decode2Asm((PBYTE)pszCode, szCurAsmBuf, &nSize, dwCurAddr);

	//显示地址
	printf("%08X    ", dwCurAddr);
	//显示机器码
	for (size_t i = 0; i < (size_t)nSize; i++)
	{
		printf("%02X ", (unsigned char)(*(pszCode + i)));
	}
	//显示汇编指令
	printf("\t%s\r\n", szCurAsmBuf);

	//保存当前行汇编指令信息
	strcpy(m_CurCode.szCurAsmBuf, szCurAsmBuf); //汇编指令
	m_CurCode.nCodeSize = nSize;                //指令长度
	m_CurCode.pCurCodeEntryAddress += nSize;    //下一条指令地址
}

/*
功能：显示10行反汇编
参数：
DWORD dwAddr 用户指定的反汇编开始地址
*/
void CMyDbg::ShowCurLineAsm(DWORD dwAddr)
{
	int i, j, k;
	char szBuf[128] = { 0 };
	UINT nCodeSize;
	char szCude[128] = { 0 };
	char szInt3Code[8] = { 0 };
	char szFunName[32] = { 0 };
	static unsigned int sOldEip = 0; //旧的EIP指令指针位置
	//记录当前的地址
	if (dwAddr == 0) //未指定起始地址，需确定u命令起始地址
	{
		//未记录u命令起始地址，
		if (m_URunAddr == 0)
		{
			//取之前记录的当前行代码地址为起始地址
			m_URunAddr = m_CurCode.pStartAddress;
			//将记录的u命令起始地址赋值给临时eip变量
			sOldEip = m_URunAddr;
		}
		//存在记录的u命令起始地址，与当前epi比较
		if (sOldEip != m_Context.Eip)
		{
			m_URunAddr = m_CurCode.pCurCodeEntryAddress;//之前记录的代码下一行地址开始执行
			sOldEip = m_Context.Eip;//将当前eip保存到临时eip
		}
	}
	else
		m_URunAddr = dwAddr; //转到用户指定的地址，执行u命令

	//显示10行反汇编
	for (i = 0; i < 10; i++)
	{
		memset(szCude, 0, sizeof(szCude));
		memset(szBuf, 0, sizeof(szBuf));
		memset(szInt3Code, 0, sizeof(szInt3Code));
		memset(szFunName, 0, sizeof(szFunName));

		//取指令
		::ReadProcessMemory(m_hProcess, (PVOID)m_URunAddr, szCude, sizeof(szCude), NULL);
		//如果当前的是软断点 还原成原来的代码输出
		if (TRUE == FindBpInt3(m_URunAddr, szInt3Code))
		{
			szCude[0] = szInt3Code[0];
		}
		//反汇编
		Decode2Asm((PBYTE)szCude, szBuf, &nCodeSize, m_URunAddr);

		//格式化输出： 地址    机器码    反汇编
		printf("%08X  ", m_URunAddr);
		for (j = 0; j < (int)nCodeSize; j++)
		{
			printf("%02X ", (unsigned char)(*(szCude + j)));
		}
		j = 20 - (int)nCodeSize - j * 2;
		for (k = 0; k <= j; k++)
		{
			printf(" ");
		}
		printf("%-30s \r\n", szBuf);

		//解析导出表，显示DLL模块和导出函数
		ShowExportFunc(szBuf);

		m_URunAddr += nCodeSize; //向后移动，记录地址
	}
}

void CMyDbg::DisplayRegisterInfo()
{
	printf("EAX = %08X ", m_Context.Eax);
	printf("ECX = %08X ", m_Context.Ecx);
	printf("EDX = %08X ", m_Context.Edx);
	printf("EBX = %08X ", m_Context.Ebx);
	printf("ESP = %08X \r\n", m_Context.Esp);
	printf("EBP = %08X ", m_Context.Ebp);
	printf("ESI = %08X ", m_Context.Esi);
	printf("EDI = %08X ", m_Context.Edi);
	printf("EIP = %08X \r\n", m_Context.Eip);
	printf("CS=%04X  DS=%04X  ES=%04X  SS=%04X  FS=%04X\t",
		m_Context.SegCs, m_Context.SegDs,
		m_Context.SegEs, m_Context.SegSs,
		m_Context.SegFs);
	printf("OF IF TF SF ZF AF PF CF\r\n");
	printf("\t\t\t\t\t\t%02d ", (m_Context.EFlags & _FLAG_OF) ? 1 : 0);
	printf("%02d ", (m_Context.EFlags & _FLAG_IF) ? 1 : 0);
	printf("%02d ", (m_Context.EFlags & _FLAG_TF) ? 1 : 0);
	printf("%02d ", (m_Context.EFlags & _FLAG_SF) ? 1 : 0);
	printf("%02d ", (m_Context.EFlags & _FLAG_ZF) ? 1 : 0);
	printf("%02d ", (m_Context.EFlags & _FLAG_AF) ? 1 : 0);
	printf("%02d ", (m_Context.EFlags & _FLAG_PF) ? 1 : 0);
	printf("%02d \r\n", (m_Context.EFlags & _FLAG_CF) ? 1 : 0);
}

void CMyDbg::ShowContextAsm()
{
	char szCurCode[32] = { 0 };
	m_CurCode.pCurCodeEntryAddress = (DWORD)m_Context.Eip;
	//取指令
	::ReadProcessMemory(m_hProcess, (unsigned char*)m_Context.Eip, &szCurCode, sizeof(szCurCode), NULL);
	m_CurCode.szCurLineCode[0] = szCurCode[0];
	m_CurCode.szCurLineCode[1] = szCurCode[1];

	char szCurAsmBuf[256] = { 0 }; //汇编指令
	UINT nSize = 0;                //指令长度

	//反汇编
	Decode2Asm((PBYTE)szCurCode, szCurAsmBuf, &nSize, m_Context.Eip);

	//显示地址
	printf("%08X    ", m_Context.Eip);
	//显示机器码
	for (size_t i = 0; i < (size_t)nSize; i++)
	{
		printf("%02X ", (unsigned char)(*(szCurCode + i)));
	}
	//显示汇编指令
	printf("\t%s\r\n", szCurAsmBuf);
}

/*
功能：接收过滤用户输入的数据
参数：
std::string szBuff：字符缓冲区
*/
void GetInput(std::string& szBuff)
{
	while (true)
	{
		puts("请输入要修改的数据:");
		std::getline(std::cin, szBuff);
		int index = 0;
		if (!szBuff.empty())
		{
			//忽略空格
			while ((index = szBuff.find(' ', index)) != std::string::npos)
			{
				szBuff.erase(index, 1);
			}
			if (szBuff.size() % 2 != 0)
			{
				printf("数据有误，请重新输入\r\n");
				continue;
			}
			break;
		}
		else
		{
			puts("未输入数据\r\n");
			break;
		}
	}
}

/*
功能：修改内存数据
参数：
DWORD dwAddr：要修改内存数据的地址
*/
void CMyDbg::EditMemData(DWORD dwAddr)
{
	std::string szBuff;
	//获取用户输入
	GetInput(szBuff);

	DWORD dwBytesToWrite = 0;
	//打开调试进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE,
		m_Event.dwProcessId);
	if (!hProcess)
		return;

	//遍历输入，准备修改写入数据
	for (int i = 0, j = 0; i < szBuff.size(); i = i + 2, j++)
	{
		char szCode[3] = { 0 }; //0x??
		memcpy(szCode, szBuff.c_str() + i, 2);   //取2个字符
		int nByte = std::strtoul(szCode, 0, 16); //将字符串转换成unsigned long(无符号长整型数)
		char Code = *((char*)& nByte); //取得转换后的值
		//将数据写进去
		BOOL res = WriteProcessMemory(hProcess,
			(LPVOID)(dwAddr + j),
			&Code,
			sizeof(Code),
			&dwBytesToWrite);
		if (!res)
		{
			printf("%s 修改失败\r\n", szCode);
			CloseHandle(hProcess);
			return;
		}
		else
		{
			printf("%s 修改成功\r\n", szCode);
		}
	}
	CloseHandle(hProcess);
	return;
}

BOOL StrConvDec(char* pSrc, OUT unsigned int& nDec)
{
	int nLen = strlen(pSrc);
	unsigned int nSum = 0;

	if (nLen > 8)
	{
		nDec = NULL;
		return FALSE;
	}

	for (int i = nLen - 1, j = 0; i >= 0; i--, j++)
	{
		if (pSrc[i] >= '0' && pSrc[i] <= '9')
		{
			nSum += (pSrc[i] - '0') * (unsigned int)pow(16, j);
			continue;
		}

		if (pSrc[i] >= 'a' && pSrc[i] <= 'f')
		{
			nSum += (pSrc[i] - 'a' + 10) * (unsigned int)pow(16, j);
			continue;
		}

		if (pSrc[i] >= 'A' && pSrc[i] <= 'F')
		{
			nSum += (pSrc[i] - 'A' + 10) * (unsigned int)pow(16, j);
			continue;
		}

		//nDec = NULL;
		//return FALSE;
	}

	nDec = nSum;
	return TRUE;
}

//void ltrim(char* s)
//{
//	char* s_tmp = s;
//
//	while (isblank(*s_tmp)) /* isblank() detects spaces and tabs. */
//	{
//		++s_tmp;
//	}
//
//	memmove(s, s_tmp, s_tmp - s); /* Please note the use of memmove() here, as it
//									 allows the use of overlapping memory areas,
//									 which is not allowed for memcpy().*/
//}

void ltrim(char* s)
{
	char* p;
	p = s;
	while (*p == ' ' || *p == '\t') { *p++; }
	strcpy(s, p);
}


/*
功能：解析用户输入的命令
参数：
const char* pszCmd：命令
*/
void CMyDbg::HandlerCommand(const char* pszCmd)
{
	//支持的命令操作
	const char* pStrCmd[] = { "dd", "u", "r", "t",
							  "p", "g", "e" , "q", "?",    //一般命令
							  "bp", "bpl", "bpc"           //所有INT3断点命令
							  "bm", "bml", "bmpl", "bmc", //所有内存断点命令
							  "bh", "bhl", "bhc", //所有硬件断点命令
							  "ls", "es",                 //导入导出
							  "Ml", "trace", "cls", "help" //其他
	};

	char strGetCmdBuf[256];        //接收用户输入的命令
	std::string strCurCmd;        //拷贝用户输入的命令

	//英文1到10的缩写分别是:1st、2nd、3rd、4th、5th、6th、7th、8th、9th、10th
	//命令形式，仅用于举例，不做真实参考，具体命令，见命令总结
	//bp xxxxx
	//lbpi /del xxxxx
	char strCmd1st[256];           //第一个空格前的命令，实际操作指令，如d , u, r, t
	char strCmd2nd[256];           //第一个空格到第二个空格之间的命令，地址或者地址-地址
	char strCmd3rd[256];           //第二个空格后的命令，字节长度或者地址
	char strCmd4rd[256];           //第三个空格后的命令，字节长度或者地址

	fflush(stdin);
	while (TRUE) {
		memset(strGetCmdBuf, 0, 256);
		memset(strCmd1st, 0, 256);
		memset(strCmd2nd, 0, 256);
		memset(strCmd3rd, 0, 256);
		memset(strCmd4rd, 0, 256);
		//处理命令临时变量
		size_t i = 0;     //标记总的命令长度
		size_t ch = 0;    //接收字符
		size_t j = 0;     //第一个空格位置
		size_t k = 0;     //第二个空格位置
		size_t l = 0;     //第三个空格位置
		size_t n = 0;     //实际操作指令，所在命令表中的位置

		size_t nBeginAddr = 0;    //标记开始地址, 对应两段中的地址，或者三段中的开始地址
		size_t nEndAddr = 0;      //标记结束地址，对应三段中的结束地址
		size_t nLenOrAddr = 0;    //标记命令第二个空格到结束的数据，字节长度或者地址
		size_t nBpHardType = 0;       //硬件断点类型，e(执行)/w(写入)/a(访问)
		size_t nBpHardLen = 0;       //硬件断点试长度，1/2/4

		fflush(stdin);
		printf("\r\n");
		//提示用户，输入命令
		printf("cmd: ");

		//判断是否从脚本中获取命令
		if (bSelIptFlag)
		{
			if (m_cmdIptList.empty())
			{
				m_IptScpFinish = TRUE;//完成导入
				bSelIptFlag = FALSE;  //复位
				break;
			}
			//取指令并弹出
			strcpy(strGetCmdBuf, m_cmdIptList.begin()->data());
			m_cmdIptList.pop_front();
			printf("%s \r\n", strGetCmdBuf);
		}
		else { //直接输入
			//接收命令
			for (i = 0; (i < 256) && ((ch = getchar()) != EOF) && (ch != '\n'); i++)
			{
				strGetCmdBuf[i] = (char)ch;
			}
		}

		//过滤
		ltrim(strGetCmdBuf);
		//保存命令
		strCurCmd = strGetCmdBuf;
		//check command
		if (strCurCmd.empty())
		{
			fflush(stdin);
		}

		BOOL bFlag = FALSE;
		//取命令
		for (i=0; i < strlen(strGetCmdBuf); i++)
		{
			if (strGetCmdBuf[i] == ' ')
			{
				if (j == 0)
					j = i; //第一个空格位置
				else if (k == 0)
					k = i; //第二个空格位置
				else if (l == 0)
					l = i; //第三个空格位置
				else
				{
					printf("指令格式有误，请重新输入\r\n");
					fflush(stdin);
					bFlag = TRUE;
				}
			}
		}

		if (bFlag)
			continue;


		//分解命令
		if ((j != 0) && (k == 0) && (l == 0)) //分解两段命令，类似bp xxxxx
		{
			//分解第一个空格前的命令
			memcpy(strCmd1st, strGetCmdBuf, j);
			//分解第一个空格后，第二个空格前的或者到结束的数据
			memcpy(strCmd2nd, &strGetCmdBuf[j + 1], i - j - 1);
			//字符转整形
			StrConvDec(strCmd2nd, nBeginAddr);
		}
		else if ((k != 0) && (l == 0)) //分解三段命令，类似lbpi /del xxxxxx-xxxxxx
		{
			//分解第一个空格前的命令
			memcpy(strCmd1st, strGetCmdBuf, j);
			//分解第一个空格后，第二个空格前的数据
			memcpy(strCmd2nd, &strGetCmdBuf[j + 1], k - j - 1);
			StrConvDec(strCmd2nd, nBeginAddr);
			//分解第二个空格后，到结束的数据
			memcpy(strCmd3rd, &strGetCmdBuf[k + 1], i - k);
			StrConvDec(strCmd3rd, nLenOrAddr);
		}
		else if (l != 0)//分解四段命令，类似bh addr len type
		{
			//分解第一个空格前的命令
			memcpy(strCmd1st, strGetCmdBuf, j);
			//分解第一个空格后，第二个空格前的数据
			memcpy(strCmd2nd, &strGetCmdBuf[j + 1], k - j - 1);
			StrConvDec(strCmd2nd, nBeginAddr); //参数1
			//分解第二个空格后，第三个空格前的数据
			memcpy(strCmd3rd, &strGetCmdBuf[k + 1], l - k - 1);
			StrConvDec(strCmd3rd, nLenOrAddr); //参数2
			//分解第三个空格后，第四个空格前的数据
			memcpy(strCmd4rd, &strGetCmdBuf[l + 1], i - l);
			StrConvDec(strCmd4rd, nBpHardLen); //参数3

		}
		else //单条命令，类似d, u, r ...
		{
			strcpy(strCmd1st, strGetCmdBuf);
		}
		strCmd1st[9] = '\0';
		strCmd2nd[19] = '\0';
		strCmd3rd[19] = '\0';
		strCmd4rd[19] = '\0';

		//派发命令
		if (strcmp(strCmd1st, "dd") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			DisplayMemData(nBeginAddr);//打印内存数据
			continue;
		}
		else if (strcmp(strCmd1st, "u") == 0)
		{
			SaveCmd(strCurCmd);//保存命令
			ShowCurLineAsm(nBeginAddr); //显示反汇编
			continue;
		}
		else if (strcmp(strCmd1st, "r") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			DisplayRegisterInfo(); //显示寄存器环境
			ShowContextAsm(); //显示反汇编
			continue;
		}
		else if (strcmp(strCmd1st, "e") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			EditMemData(nBeginAddr); //修改内存数据
			continue;
		}
		else if (strcmp(strCmd1st, "t") == 0)
		{
			SaveCmd(strCurCmd); //保存命令到链尾
			StepInProc(); //单步步入
			break;
		}
		else if (strcmp(strCmd1st, "p") == 0)
		{
			SaveCmd(strCurCmd); //保存命令到链尾
			StepJumpProc(); //单步步过
			break;
		}
		else if (strcmp(strCmd1st, "g") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			GRunProc(nBeginAddr);  //直接执行
			return;
		}
		else if (strcmp(strCmd1st, "?") == 0 ||
			strcmp(strCmd1st, "help") == 0)
		{
			SaveCmd(strCurCmd); //保存命令到链尾
			ShowDebugHelp(); //显示帮助
			continue;
		}
		else if (strcmp(strCmd1st, "q") == 0)
		{
			exit(0);
			return;
		}
		else if (strcmp(strCmd1st, "bp") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			SetBpInt3(nBeginAddr); //设置int3断点
			continue;
		}
		else if (strcmp(strCmd1st, "bpl") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			ShowBpint3(); //显示int3断点列表
			continue;
		}
		else if (strcmp(strCmd1st, "bpc") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			//nLength当作断点使用
			if (TRUE == DelBpInt3(nBeginAddr))
				m_UI.Output("\t断点删除成功!\r\n");
			else
				m_UI.Message("\t断点删除失败!\r\n");
		}
		else if (strcmp(strCmd1st, "bh") == 0)
		{
			//check
			if (!((strcmp(strCmd3rd, "e") == 0) ||
				(strcmp(strCmd3rd, "w") == 0) ||
				(strcmp(strCmd3rd, "a") == 0))) {
				m_UI.Message("\t硬件断点类型错误!\r\n");
				continue;
			}
			if ((strcmp(strCmd3rd, "w") == 0) ||
				(strcmp(strCmd3rd, "a") == 0)) {
				if (!((nBpHardLen == 1) ||
					(nBpHardLen == 2) ||
					(nBpHardLen == 4))) {
					m_UI.Message("\t硬件断点长度错误!\r\n");
					continue;
				}
			}

			//
			if (strcmp(strCmd4rd, "e") == 0) {
				nBpHardType = 0;
			}
			if (strcmp(strCmd4rd, "w") == 0) {
				nBpHardType = 1;
			}
			if (strcmp(strCmd4rd, "a") == 0) {
				nBpHardType = 3;
			}

			SaveCmd(strCurCmd); //保存命令
			DelBpInt3(nBeginAddr);//删除这个地址上的int3断点
			//硬件断点	bh	Bp Hard　	地址	断点长度(1, 2, 4)	e(执行) / w(写入) / a(访问)
			SetBpHard(nBeginAddr, nBpHardType, nBpHardLen);
			continue;
		}
		else if (strcmp(strCmd1st, "bhl") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			ShowBpHard();//硬件断点列表
			continue;
		}
		else if (strcmp(strCmd1st, "bhc") == 0)
		{
			SaveCmd(strCurCmd); //保存命令
			DelBpHard(nBeginAddr);//删除硬件断点
			continue;
		}
		else if (strcmp(strCmd1st, "ls") == 0)
		{
			LoadScript();//导入脚本
			continue;
		}
		else if (strcmp(strCmd1st, "es") == 0)
		{
			ExportScript();//导出脚本
			continue;
		}
		else if (strcmp(strCmd1st, "Ml") == 0)
		{
			ShowModuleList();
			continue;
		}
		else if (strcmp(strCmd1st, "bm") == 0)
		{
			//bm	地址	长度	r(读) / w(写)
			//int3断点与内存断点重合，先移除，后边再恢复
			DelBpInt3(nBeginAddr);
			//设置内存断点
			SetBpMem(nBeginAddr, nLenOrAddr, strCmd4rd);
			//保存命令
			SaveCmd(strCurCmd);
			continue;
		}
		else if (strcmp(strCmd1st, "bml") == 0)
		{
			ShowBpMemList(); //内存断点列表
			continue;
		}
		else if (strcmp(strCmd1st, "bmc") == 0)
		{
			DelBpMem(nBeginAddr); //删除内存断点
			continue;
		}
		else
		{
			printf("输入有误，请重新输入。 <? or help> \r\n");
			continue;
		}
	}

	return;
}

void CMyDbg::DisplayMemData(DWORD dwAddr = 0)
{
	char szHex[256];            //内存显示缓冲区大小
	static size_t snOldEip = 0; //记录上一次EIP指令指针位置

	if (dwAddr == 0) //d
	{
		if (m_dInsCurAddr == 0)
		{
			//d指令地址为空，使用入口地址
			m_dInsCurAddr = m_CurCode.pStartAddress;
			snOldEip = m_dInsCurAddr;
		}
		if (snOldEip != m_Context.Eip)
		{
			//上一次EIP指令指针位置和当前EIP指令指针位置不同，使用新的EIP指令指针地址
			m_dInsCurAddr = m_CurCode.pCurCodeEntryAddress;
			snOldEip = m_dInsCurAddr;
		}
	}
	else //d xxxxxxxx
		m_dInsCurAddr = dwAddr;

	//读调试进程内存数据
	ReadProcessMemory(m_hProcess, (LPVOID)m_dInsCurAddr, szHex, sizeof(szHex), NULL);

	//处理显示内存数据
	for (size_t i = 0; i < 8; i++)
	{
		//地址
		printf("%p  ", (void*)m_dInsCurAddr);

		//16进制数据
		for (size_t j = 0; j < 16; j++)
		{
			if (j == 7)
			{
				printf("%02X", (unsigned char)szHex[i * 16 + j]);
				printf("-");
			}
			else
				printf("%02X  ", (unsigned char)szHex[i * 16 + j]);
		}

		//ASCII
		for (size_t k = 0; k < 16; k++)
		{
			//处理3个特殊字符(0A 0D 09)，影响界面显示输出效果
			if ((unsigned char)szHex[i * 16 + k] == NULL ||
				(unsigned char)szHex[i * 16 + k] == 0xFF ||
				(unsigned char)szHex[i * 16 + k] == 0x0A ||  //0A ASC码为换行
				(unsigned char)szHex[i * 16 + k] == 0x0D ||  //0D ASC码为回车
				(unsigned char)szHex[i * 16 + k] == 0x09 ||  //09 ASC码为TAB健
				(unsigned char)szHex[i * 16 + k] == 0x07)    //07 ASC码响铃
			{
				printf(".");
			}
			else
				printf("%c", (unsigned char)szHex[i * 16 + k]);
		}

		//换行
		printf("\r\n");
		m_dInsCurAddr += 16;
	}
}

/*
功能：显示帮助
*/
void CMyDbg::ShowDebugHelp()
{
	//{ "dd", "u", "r", "t",
	//"p", "g", "e", "q", "?",    //一般命令
	//"bp", "bpl", "bpc"           //所有INT3断点命令
	//"bm", "bml", "bmpl", "bmc", //所有内存断点命令
	//"bh", "bhl", "bhc", //所有硬件断点命令
	//"ls", "es",                 //导入导出
	//"Ml", "trace", "cls", "help" //其他
	printf("------------------------------------------------\r\n");
	printf("\t\t Help Menu\r\n");
	printf("\r\n");

	printf("\r\n");
	printf("一般命令：\r\n");
	printf("\t<t> 单步步入\r\n");
	printf("\t<p> 单步步过\r\n");
	printf("\t<r> 查看寄存器状态\r\n");
	printf("\t<e addr> 修改内存数据\r\n");
	printf("\t    - <输入内容格式> 空格分隔\r\n");
	printf("\t<? or help> 查看帮助\r\n");
	printf("\t<dd addr> 查看内存\r\n");
	printf("\t<g> 执行程序\r\n");
	printf("\t<u addr> 查看反汇编\r\n");
	printf("\t<g addr> 执行到指定地址\r\n");
	printf("\t<ls> 导入脚本\r\n");
	printf("\t<es> 导入脚本\r\n");
	printf("\t<Ml> 查看模块\r\n");

	printf("\r\n");
	printf("一般断点：\r\n");
	printf("\t<bp addr> INT3断点\r\n");
	printf("\t<bpl> 查看所有INT3断点\r\n");
	printf("\t<bpc num> 删除INT3断点\r\n");

	printf("\r\n");
	printf("内存断点：\r\n");
	printf("\t<bm addr len type> r(读)/w(写)\r\n");
	printf("\t    - <type> r(读)/w(写)\r\n");
	printf("\t<bml> 内存断点列表\r\n");
	printf("\t<bmpl> 分页断点列表\r\n");
	printf("\t<bmc num> 删除内存断点\r\n");

	printf("\r\n");
	printf("硬件断点：\r\n");
	printf("\t<bh addr len type> 硬件断点\r\n");
	printf("\t    - <len> 断点长度(1,2,4)\r\n");
	printf("\t    - <type> e(执行)/w(写入)/a(访问)\r\n");
	printf("\t<bhl> 硬件断点列表\r\n");
	printf("\t<bhc num> 删除硬件断点\r\n");
	printf("\t    - <num> 序号(1,2,3,4)\r\n");

	printf("\r\n");
	printf("其他：\r\n");
	printf("<cls> 清屏\r\n");
	printf("<q> 退出\r\n");

	printf("------------------------------------------------\r\n");
}

/*
功能：保存命令
参数：
std::string strCmd 命令
*/
void CMyDbg::SaveCmd(std::string strCmd)
{
	m_cmdRecordList.push_back(strCmd);
}

//单步步入<T>
void CMyDbg::StepInProc()
{
	m_Context.EFlags |= 0x100;
	m_EpnFlag.bSetInFlag = TRUE;
}

//t 单步步入，处理
void CMyDbg::DebugStepInProc()
{
	char szCurCode[32] = { 0 };
	m_CurCode.pCurCodeEntryAddress = (DWORD)m_Context.Eip;
	//取指令
	::ReadProcessMemory(m_hProcess, (unsigned char*)m_CurCode.pCurCodeEntryAddress, &szCurCode, sizeof(szCurCode), NULL);
	m_CurCode.szCurLineCode[0] = szCurCode[0];
	m_CurCode.szCurLineCode[1] = szCurCode[1];

	//打印寄存器环境
	DisplayRegisterInfo();
	//显示汇编指令
	DisplayAssembly(szCurCode, m_CurCode.pCurCodeEntryAddress);
	//t复位
	m_EpnFlag.bSetInFlag = FALSE;
}

//p 单步步过
void CMyDbg::StepJumpProc()
{
	//取消单步进入 根据第一条指令再次选择步入或步过
	m_EpnFlag.bSetInFlag = FALSE;
	//调用反汇编引擎进行特殊指令处理
	char szRcodeTmp[16] = { 0 };
	//对每条指令的首个字节判断
	GetJumpInstruction((BYTE)m_CurCode.szCurLineCode[0], szRcodeTmp);
	if (szRcodeTmp[0] != NULL)
	{
		m_Context.EFlags |= 0x100;
		m_EpnFlag.bSetInFlag = TRUE;
		return;
	}

	if ((unsigned char)m_CurCode.szCurLineCode[0] == 0xf3 ||
		(unsigned char)m_CurCode.szCurLineCode[0] == 0xf2 ||
		(unsigned char)m_CurCode.szCurLineCode[0] == 0xe8 ||
		(unsigned char)m_CurCode.szCurLineCode[0] == 0x9a ||
		(unsigned char)m_CurCode.szCurLineCode[0] == 0xc3 ||
		(unsigned char)m_CurCode.szCurLineCode[0] == 0xff)
	{
		if (m_CurCode.szCurLineCode[1] == 0x25 ||
			(unsigned char)m_CurCode.szCurLineCode[0] == 0xc3)
		{
			//jump指令  <==> 单步进入处理
			m_Context.EFlags |= 0x100;
			m_EpnFlag.bSetInFlag = TRUE;
			return;
		}
		else
		{   //遇到call指令，在call下一行指令设置int3断点
			ReadProcessMemory(m_hProcess,
				(LPVOID)m_CurCode.pCurCodeEntryAddress,
				&m_GRunCode,
				sizeof(BYTE),
				NULL);
			WriteProcessMemory(m_hProcess,
				(LPVOID)m_CurCode.pCurCodeEntryAddress,
				(LPVOID)& _0xCC,
				sizeof(BYTE),
				NULL);
			m_CurCode.szSrcCode = m_GRunCode;
			m_EpnFlag.bGRunFlag = TRUE;
			return;
		}
	}
	else
	{ //p
		m_Context.EFlags |= 0x100;
		m_EpnFlag.bSetJumpFlag = TRUE;
		return;
	}
}

/*
功能：单步步过
*/
void CMyDbg::DebugStepJumpProc()
{
	//一次P处理
	m_EpnFlag.bSetJumpFlag = FALSE;

	char szCurCode[32] = { 0 };
	//取指令
	ReadProcessMemory(m_hProcess,
		(unsigned char*)m_CurCode.pCurCodeEntryAddress,
		&szCurCode,
		sizeof(szCurCode),
		NULL);
	//保存当前代码
	m_CurCode.szCurLineCode[0] = szCurCode[0];
	m_CurCode.szCurLineCode[1] = szCurCode[1];

	//打印寄存器环境
	DisplayRegisterInfo();
	//显示汇编指令
	DisplayAssembly(szCurCode, m_CurCode.pCurCodeEntryAddress);
}

/*
功能：直接执行或执行到某地址
参数：
std::string strCmd 命令
DWORD dwAddr 目标地址
*/
void CMyDbg::GRunProc(DWORD dwAddr = 0)
{
	if (dwAddr != 0)
	{
		//触发G运行的判断标志
		m_EpnFlag.bGRunFlag = TRUE;
		//设置目标地址为新的起始执行地址
		m_CurCode.pCurCodeEntryAddress = dwAddr;
		//保存原指令
		m_GRunCode = SetBreakPoint(m_hProcess, (LPVOID)dwAddr, _0xCC);
	}
	return;
}

int CMyDbg::DbgStartProcess(const char* pszFilePath)
{
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);

	if (!CreateProcess(pszFilePath,
		NULL,
		NULL,
		NULL,
		TRUE,
		DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS,
		NULL,
		NULL,
		&si,
		&m_ProcessInfo))
	{
		m_UI.Message("Debug Error err:%d", GetLastError());
		system("pause");
		return 0;
	}

	m_UI.Output("Debug OK\n");

	CloseHandle(m_ProcessInfo.hProcess);
	CloseHandle(m_ProcessInfo.hThread);


	DWORD dwContinueStatus = DBG_CONTINUE;
	//等待调试器事件
	while (WaitForDebugEvent(&m_Event, INFINITE)) {
		//分发处理异常事件
		dwContinueStatus = HandlerEvent();

		//向系统返回处理结果
		ContinueDebugEvent(m_Event.dwProcessId, m_Event.dwThreadId, dwContinueStatus);
	}

	return 0;
}

DWORD CMyDbg::HandlerEvent()
{
	DWORD dwContinueStatus = DBG_CONTINUE;

	switch (m_Event.dwDebugEventCode) {
	case EXCEPTION_DEBUG_EVENT:
		dwContinueStatus = OnException();
		break;
	case CREATE_THREAD_DEBUG_EVENT:
		dwContinueStatus = OnCreateThread();
		break;
	case CREATE_PROCESS_DEBUG_EVENT:
		dwContinueStatus = OnCreateProcess();
		break;
	case EXIT_THREAD_DEBUG_EVENT:
		dwContinueStatus = OnExitThread();
		break;
	case EXIT_PROCESS_DEBUG_EVENT:
		dwContinueStatus = OnExitProcess();
		break;
	case LOAD_DLL_DEBUG_EVENT:
		dwContinueStatus = OnLoadDll();
		break;
	case UNLOAD_DLL_DEBUG_EVENT:
		dwContinueStatus = OnUnLoadDll();
		break;
	case OUTPUT_DEBUG_STRING_EVENT:
		dwContinueStatus = OnOutputDebugString();
		break;
	case RIP_EVENT:
		break;
	default:
		break;
	}

	return dwContinueStatus;
}

int CMyDbg::GetCPU(CONTEXT& cxt)
{
	cxt.ContextFlags = CONTEXT_ALL;
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, m_Event.dwThreadId);
	GetThreadContext(hThread, &cxt);
	CloseHandle(hThread);
	return 0;
}

int CMyDbg::SetCPU(CONTEXT& cxt)
{
	cxt.ContextFlags = CONTEXT_ALL;
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, m_Event.dwThreadId);
	//check it
	SetThreadContext(hThread, &cxt);
	CloseHandle(hThread);
	return 0;
}

DWORD CMyDbg::OnException()
{
	GetCPU(m_Context);

	DWORD dwContinueStatus = DBG_CONTINUE;
	EXCEPTION_RECORD record = m_Event.u.Exception.ExceptionRecord;

	switch (record.ExceptionCode) {
	case EXCEPTION_ACCESS_VIOLATION://内存访问异常
		dwContinueStatus = OnExceptAccess();
		break;
	case EXCEPTION_BREAKPOINT://断点异常
		m_bFirstLoadDll = FALSE;
		dwContinueStatus = OnExceptBreakPoint();
		break;
	case EXCEPTION_SINGLE_STEP://单步异常
		dwContinueStatus = OnExceptSingleStep();
		break;
	default:
		break;
	}

	SetCPU(m_Context);

	if (!m_IsInput) {
		GetCPU(m_Context);
		SetMemPageAllAccess();
		HandlerCommand();
		SetCPU(m_Context);
	}
	m_IsInput = FALSE;

	return dwContinueStatus;
}

DWORD CMyDbg::OnExceptAccess()
{
	EXCEPTION_RECORD record = m_Event.u.Exception.ExceptionRecord;
	std::list<tagBpMemInfor>::iterator iter;
	DWORD dwProtect = 0;
	char szCurCode[32] = { 0 };


	//判断内存断点是否命中
	for (iter = m_pBpMemInforList.begin();iter != m_pBpMemInforList.end();iter++) 
	{
		//printf("  %-p %-p %-p %-p \r\n",
		//	m_Context.Eip,
		//	record.ExceptionInformation[1],
		//	iter->dwAddr,
		//	iter->dwAddr + iter->nSize);
		//命中断点
		if ((record.ExceptionInformation[1] >= iter->dwAddr) &&
			(record.ExceptionInformation[1] <= iter->dwAddr + iter->nSize) &&
			(iter->bDelFlag == TRUE))
		{
			//恢复内存属性
			BOOL bRet = VirtualProtectEx(m_hProcess,
				(void*)iter->dwAddr,
				1,
				m_pBpMemInforList.begin()->OldPageProtect,
				&dwProtect);

			//取指令
			ReadProcessMemory(m_hProcess,
				record.ExceptionAddress,
				szCurCode,
				sizeof(szCurCode),
				NULL);
			m_CurCode.szCurLineCode[0] = szCurCode[0];
			m_CurCode.szCurLineCode[1] = szCurCode[1];

			//断点类型
			if (iter->pBpType == BP_MEM_READ) {
				m_UI.Output("EIP: 0x%08X, 内存断点地址: 0x%08X, 类型: 访问内存\r\n",
					m_Context.Eip,
					record.ExceptionInformation[1]);
			}
			if (iter->pBpType == BP_MEM_WRITE) {
				m_UI.Output("EIP: 0x%08X, 内存断点地址: 0x%08X, 类型: 访问内存\r\n",
					m_Context.Eip,
					record.ExceptionInformation[1]);
			}
			m_CurCode.pCurCodeEntryAddress = (DWORD)record.ExceptionAddress;

			//显示环境
			DisplayRegisterInfo();
			//显示汇编指令
			DisplayAssembly(szCurCode, m_CurCode.pCurCodeEntryAddress);

			m_IsInput = FALSE;         //接收输入
			m_Context.EFlags |= 0x100; //设单步
			m_memStepFlag = TRUE;      //内存标记复位

			return DBG_CONTINUE;
		}
	}

	//没有命中
	for (auto x = m_pBpMemInforList.begin(); x != m_pBpMemInforList.end(); x++)
	{
		if (x->bDelFlag == TRUE)
		{
			//恢复原来的内存属性
			BOOL bRet = VirtualProtectEx(m_hProcess,
				(void*)x->dwAddr,
				1,
				m_pBpMemInforList.begin()->OldPageProtect,
				//x->OldPageProtect,
				&dwProtect);
			//printf(" --%d\r\n", GetLastError());
		}
	}

	//ReadProcessMemory(m_hProcess, record.ExceptionAddress, szCurCode, sizeof(szCurCode), NULL);

	//m_CurCode.pCurCodeEntryAddress = (DWORD)record.ExceptionAddress;
	m_Context.EFlags |= 0x100;
	m_memStepFlag = TRUE;
	m_IsInput = TRUE;

	return DBG_CONTINUE;
}

DWORD CMyDbg::OnExceptBreakPoint()
{
	EXCEPTION_RECORD record = m_Event.u.Exception.ExceptionRecord;

	//保存当前行机器码
	char szCurCode[32] = { 0 };

	//忽略系统断点
	//入口点INT3断点处理
	if (record.ExceptionAddress == m_DbgProcessInfo.lpStartAddress)
	{
		//取消断点
		SetBreakPoint(m_DbgProcessInfo.hProcess,
			m_DbgProcessInfo.lpStartAddress,
			m_oepCode);
		m_Context.Eip--;//陷阱，EIP指令指针位置回退1
		//读取当前行指令
		ReadProcessMemory(m_DbgProcessInfo.hProcess,
			m_DbgProcessInfo.lpStartAddress,
			&szCurCode,
			sizeof(szCurCode),
			NULL);

		DisplayRegisterInfo();
		DisplayAssembly(szCurCode, (DWORD)m_DbgProcessInfo.lpStartAddress);
	}
	else if (m_EpnFlag.bGRunFlag) //CALL处理
	{
		m_EpnFlag.bGRunFlag = FALSE;
		//写回原来的指令
		WriteProcessMemory(m_hProcess,
			(LPVOID)m_CurCode.pCurCodeEntryAddress,
			&m_GRunCode,
			sizeof(char),
			NULL);
		m_Context.Eip--;
		ReadProcessMemory(m_hProcess,
			(LPVOID)m_CurCode.pCurCodeEntryAddress,
			szCurCode,
			sizeof(szCurCode),
			NULL);
		//1.分析CALL或JMP特殊指令 2.EIP特殊处理
		m_CurCode.szCurLineCode[0] = szCurCode[0];
		m_CurCode.szCurLineCode[1] = szCurCode[1];

		DisplayRegisterInfo();
		DisplayAssembly(szCurCode, m_CurCode.pCurCodeEntryAddress);
	}
	else //默认bp命令(int 3)处理
	{
		std::list<tagBpInt3>::iterator iter;
		//遍历用户主动设置的int3断点
		for (iter = m_bpInt3List.begin();
			iter != m_bpInt3List.end();
			iter++) {
			//保存当前int3地址信息 重置后继续设置CC
			m_BpNode = iter;
			if ((record.ExceptionAddress == (void*)iter->dwAddr) &&
				(TRUE == iter->bDelFlag))
			{
				//取消断点
				SetBreakPoint(m_hProcess, (LPVOID)iter->dwAddr, iter->btCode);
				//eip回退
				m_Context.Eip--;
				//读原来的指令
				ReadProcessMemory(m_hProcess,
					(LPVOID)iter->dwAddr,
					szCurCode,
					sizeof(szCurCode),
					NULL);
				//修正eip，赋值oep
				m_CurCode.pCurCodeEntryAddress = m_Context.Eip;
				//1.分析CALL或JMP特殊指令 2.EIP特殊处理
				m_CurCode.szCurLineCode[0] = szCurCode[0];
				m_CurCode.szCurLineCode[1] = szCurCode[1];

				m_UI.Output("当前EIP：0x%08X 触发INT3断点\r\n", m_Context.Eip);
				DisplayRegisterInfo();//打印寄存器环境
				DisplayAssembly(szCurCode, iter->dwAddr);//显示汇编指令

				m_IsInput = TRUE;

				//置单步
				m_Context.EFlags |= 0x100;
				m_EpnFlag.bSetInt3Flag = TRUE;
				break;

			}
		}

		//如果不是自己下的断点，则忽略
		if (iter == m_bpInt3List.end())
		{
			m_IsInput = TRUE;//将跳过输入

			return DBG_CONTINUE;
		}

		//
	}

	return DBG_CONTINUE;
}

DWORD CMyDbg::OnExceptSingleStep()
{
	//内存断点重设
	if (m_memStepFlag == TRUE)
	{
		DWORD dwProtect = 0;
		
		//恢复内存断点
		std::list<tagBpMemInfor>::iterator iter;
		for (iter = m_pBpMemInforList.begin();
			iter != m_pBpMemInforList.end();
			iter++)
		{
			
			if (iter->pBpType == BP_MEM_READ) {
				VirtualProtectEx(m_hProcess,
					(void*)iter->dwAddr,
					1,
					PAGE_NOACCESS,
					&dwProtect);
			}

			if (iter->pBpType == BP_MEM_WRITE) {
				VirtualProtectEx(m_hProcess,
					(void*)iter->dwAddr,
					1,
					PAGE_EXECUTE_READ,
					&dwProtect);
			}
		}
		//m_CurCode.pCurCodeEntryAddress = m_Context.Eip;
		m_memStepFlag = FALSE;//内存标志清空
		m_IsInput = TRUE;     //忽略输入
	}

	//重置INT3断点
	if (TRUE == m_EpnFlag.bSetInt3Flag)
	{
		SetMemPageAllAccess();
		SetBreakPoint(m_ProcessInfo.hProcess, (LPVOID)m_BpNode->dwAddr, _0xCC);
		SetMemPageNoAccess();
		m_EpnFlag.bSetInt3Flag = FALSE;
	}

	//硬件断点处理重设
	if (m_EpnFlag.bHardWareSetinFlag == TRUE)
	{
		m_Context.Dr7 = m_HWBPointValue;
		m_HWBPointValue = 0;
		m_EpnFlag.bHardWareSetinFlag = FALSE;
	}
	//硬件断点处理
	if (m_EpnFlag.bHardWareFlag == TRUE)
	{
		SetMemPageAllAccess();
		DebughardWareProc();
		SetMemPageNoAccess();
	}


	//单步步入处理
	if (m_EpnFlag.bSetInFlag == TRUE)
	{
		SetMemPageAllAccess();
		DebugStepInProc();
		SetMemPageNoAccess();

		m_IsInput = FALSE;     //接收输入
	}

	//单步步过处理
	if (m_EpnFlag.bSetJumpFlag == TRUE)
	{
		SetMemPageAllAccess();
		DebugStepJumpProc();
		SetMemPageNoAccess();

		m_IsInput = FALSE;     //接收输入
	}

	//导入脚本
	if (m_IptScpFinish)
	{
		m_IsInput = FALSE;
	}

	return DBG_CONTINUE;
}

/*
功能：设置一般断点
参数：
DWORD dwAddr：目标断点地址
*/
void CMyDbg::SetBpInt3(DWORD dwAddr)
{
	//硬件断点和INT3断点重合处理  迫使INT3断点失效
	if (dwAddr == m_Context.Dr0 ||
		dwAddr == m_Context.Dr1 ||
		dwAddr == m_Context.Dr2 ||
		dwAddr == m_Context.Dr3
		)
	{
		HandlerBpInt3(dwAddr);
		DelBpInt3(dwAddr);
	}
	else
		HandlerBpInt3(dwAddr);
}

void CMyDbg::HandlerBpInt3(DWORD dwAddr)
{
	BYTE szBuf;
	tagBpInt3 pInt3Point;
	memset(&pInt3Point, 0, sizeof(pInt3Point));
	char CodeTmp[8] = { 0 };

	//取得原来指令
	ReadProcessMemory(m_ProcessInfo.hProcess,
		(unsigned char*)dwAddr,
		&szBuf,
		sizeof(char),
		NULL);
	//设置cc断点
	SetBreakPoint(m_ProcessInfo.hProcess, (LPVOID)dwAddr, _0xCC);
	//保存原来指令
	pInt3Point.btCode = szBuf;
	pInt3Point.dwAddr = (DWORD)dwAddr;
	pInt3Point.bDelFlag = TRUE;
	//检测重复
	if (FALSE == FindBpInt3(dwAddr, CodeTmp))
	{
		m_bpInt3List.push_back(pInt3Point);
		m_UI.Message("\t新增INT3中断, 地址 : 0x % 08x\r\n", dwAddr);
	}
	else
	{
		//考虑INT3断点反复下在同一地址 反复取消以及和其他断点组合下的情况 恢复标记位 不在增加新结点
		std::list<tagBpInt3>::iterator iter;
		for (iter = m_bpInt3List.begin(); iter != m_bpInt3List.end(); iter++)
		{
			if ((iter->dwAddr == dwAddr) && //地址重合
				(FALSE == iter->bDelFlag))  //未删除状态
			{
				SetBreakPoint(m_ProcessInfo.hProcess, (LPVOID)dwAddr, _0xCC);
				//int3断点置位删除
				iter->bDelFlag = TRUE;
				return;
			}
		}
		m_UI.Message("\t已存在INT3中断, 地址 : 0x % 08x\r\n", dwAddr);
	}
}

//设置断点：m_oepCode = SetBreakPoint(hProcess,pAddr,_0xCC);
//（取消）恢复断点：SetBreakPoint(hProcess,pAddr,m_oepCode);
BYTE CMyDbg::SetBreakPoint(HANDLE hProcess, LPVOID pAddr, BYTE code, BOOL bFlag)
{
	BOOL bRet = 0;
	BOOL bReadOneByte = 0;
	DWORD dwReadBytes = 0;
	DWORD dwWriteBytes = 0;
	DWORD dwOldProtect = 0;

	//0x80000000以上的地址为系统共有区域，不能够修改
	if (pAddr >= (LPVOID)0x80000000 || pAddr == (LPVOID)0)
	{
		return code;
	}

	//获取目标地址一字节机器码
	bRet = ReadProcessMemory(hProcess, pAddr, &bReadOneByte, sizeof(BYTE), &dwReadBytes);

	//如果原来的代码和要修改的代码相同，不修改
	if (bRet == 0 || bReadOneByte == code)
	{
		return code;
	}

	//修改内存页属性为可读可写，并保存原属性
	VirtualProtectEx(hProcess, pAddr, sizeof(BYTE), PAGE_READWRITE, &dwOldProtect);
	//修改目标地址首字节机器码为0xCC
	WriteProcessMemory(hProcess, pAddr, (LPCVOID)& code, sizeof(BYTE), &dwReadBytes);
	//恢复原内存页属性
	VirtualProtectEx(hProcess, pAddr, sizeof(BYTE), dwOldProtect, &dwOldProtect);

	//返回目标地址原来的首字节机器码
	return bReadOneByte;
}

/*
功能：找int3断点
参数：
DWORD dwBpAddr 要查找的位置
char* pszCode 原来的机器码
*/
BOOL CMyDbg::FindBpInt3(DWORD dwBpAddr, OUT char* pszCode)
{
	std::list<tagBpInt3>::iterator iter;

	for (iter = m_bpInt3List.begin();
		iter != m_bpInt3List.end();
		iter++)
	{
		if (iter->dwAddr == dwBpAddr)
		{
			strcpy(pszCode, (const char*)& iter->btCode);
			return TRUE;
		}
	}
	return FALSE;
}

/*
功能：删除int3断点
参数：
DWORD dwAddr：地址或者序号
*/
BOOL CMyDbg::DelBpInt3(DWORD dwAddr)
{
	std::list<tagBpInt3>::iterator iter;
	size_t nIdx = 0;

	//遍历int3断点列表
	for (iter = m_bpInt3List.begin();
		iter != m_bpInt3List.end();
		iter++)
	{
		if (iter->dwAddr == dwAddr || nIdx + 1 == dwAddr)
		{
			//清除INT3断点，还原指令
			SetBreakPoint(m_hProcess, (LPVOID)iter->dwAddr, iter->btCode);
			//删除标记
			iter->bDelFlag = FALSE;
			m_EpnFlag.bSetInt3Flag = FALSE;
			return TRUE;
		}
		nIdx++;
	}
	return FALSE;
}

/*
功能：int3断点列表
*/
void CMyDbg::ShowBpint3()
{
	std::list<tagBpInt3>::iterator iter;
	size_t nIdx = 0;

	for (iter = m_bpInt3List.begin();
		iter != m_bpInt3List.end();
		iter++)
	{
		if ((iter->dwAddr == NULL)) //过滤无效项
			continue;

		//格式化int3断点列表输出
		m_UI.Output("%2d\t  0x%08X\t   ", nIdx + 1, iter->dwAddr);
		if (FALSE == iter->bDelFlag)
			m_UI.Output("未启用\r\n");
		else
			m_UI.Output("已启用\r\n");

		nIdx++;
	}
	return;
}

/*
功能：由硬件断点引起的失效INT3断点重设
参数：
DWORD dwAddr：断点地址
*/
BOOL CMyDbg::SetBpInt3Again(DWORD dwAddr)
{

	std::list<tagBpInt3>::iterator iter;

	for (iter = m_bpInt3List.begin();
		iter != m_bpInt3List.end();
		iter++)
	{
		if (iter->dwAddr == dwAddr && //地址相同且
			FALSE == iter->bDelFlag) //标志为失效状态
		{
			iter->bDelFlag = TRUE; //int3断点标志置位
			SetBreakPoint(m_hProcess, (LPVOID)iter->dwAddr, _0xCC);
			return TRUE;
		}
	}

	return FALSE;
}

/*
功能：选择DR寄存器设置硬件断点
参数：
DWORD SetAddress：硬件断点地址
int DR：选择哪个DR寄存器
int PointType：硬件断点类型
int nLen：设置几个长度的断点
*/
void CMyDbg::SetHardWarePoint(DWORD SetAddress, int DR, int PointType, int nLen)
{
	switch (DR)
	{
	case 1://寄存器DR0
	{
		//m_Context.Dr7 |= 0x301;                       // 11 00 00 00 01;
		m_Context.Dr0 = SetAddress;
		m_HWflag.DR0 = TRUE;  //表示被使用
		SetDR0HardWarePoint(PointType, nLen);
	}
	break;

	case 2://寄存器DR1
	{
		m_Context.Dr1 = SetAddress;
		m_HWflag.DR1 = TRUE;
		SetDR1HardWarePoint(PointType, nLen);
	}
	break;

	case 3://寄存器DR2
	{
		m_Context.Dr2 = SetAddress;
		m_HWflag.DR2 = TRUE;
		SetDR2HardWarePoint(PointType, nLen);
	}

	break;

	case 4://寄存器DR3
	{
		m_Context.Dr3 = SetAddress;
		m_HWflag.DR3 = TRUE;
		SetDR3HardWarePoint(PointType, nLen);
	}
	break;
	}
	//记录设置标志
	m_EpnFlag.bHardWareFlag = TRUE;
}

void CMyDbg::SetDR0HardWarePoint(int PointType, int nLen)
{
	//长度 1字节<00> 2字节<01> 4字节<11> 
	switch (nLen)
	{
		//1字节
	case 1:
	{
		//类型  读或写<11>=3  写<01>=1  执行<00>=0
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR0EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR0WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR0RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	//2字节
	case 2:
	{
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | TWODR0EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | TWODR0WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | TWODR0RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	//4字节	
	case 4:
	{
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR0EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR0WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR0RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void CMyDbg::SetDR1HardWarePoint(int PointType, int nLen)
{
	switch (nLen)
	{
		//1字节
	case 1:
	{
		//类型  读或写<11>=3  写<01>=1  执行<00>=0
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR1EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR1WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR1RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	//2字节
	case 2:
	{
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | TWODR1EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | TWODR1WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | TWODR1RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	//4字节	
	case 4:
	{
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR1EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR1WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR1RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void CMyDbg::SetDR2HardWarePoint(int PointType, int nLen)
{
	switch (nLen)
	{
		//1字节
	case 1:
	{
		//类型  读或写<11>=3  写<01>=1  执行<00>=0
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR2EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR2WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR2RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	//2字节
	case 2:
	{
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | TWODR2EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | TWODR2WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | TWODR2RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	//4字节	
	case 4:
	{
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR2EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR2WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR2RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void CMyDbg::SetDR3HardWarePoint(int PointType, int nLen)
{
	switch (nLen)
	{
		//1字节
	case 1:
	{
		//类型  读或写<11>=3  写<01>=1  执行<00>=0
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR3EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR3WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | ONEDR3RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	//2字节
	case 2:
	{
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | TWODR3EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | TWODR3WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | TWODR3RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	//4字节	
	case 4:
	{
		switch (PointType)
		{
			//执行
		case 0:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR3EVALUE;
			break;
			//写
		case 1:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR3WVALUE;
			break;
			//读或写
		case 3:
			m_Context.Dr7 = m_Context.Dr7 | FOURDR3RWVALUE;
			break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

/*
功能：硬件断点列表
*/
void CMyDbg::ShowBpHard()
{
	m_UI.Output("序号\t 地址\t 状态\r\n");
	m_UI.Output("[1]DR0\t  0x%08X\t %s\r\n",
		m_Context.Dr0,
		m_Context.Dr0 ? "有效" : "无效");
	m_UI.Output("[2]DR1\t  0x%08X\t %s\r\n",
		m_Context.Dr1,
		m_Context.Dr1 ? "有效" : "无效");
	m_UI.Output("[3]DR2\t  0x%08X\t %s\r\n",
		m_Context.Dr2,
		m_Context.Dr2 ? "有效" : "无效");
	m_UI.Output("[4]DR3\t  0x%08X\t %s\r\n",
		m_Context.Dr3,
		m_Context.Dr3 ? "有效" : "无效");
}

/*
功能：设置硬件断点
参数：
DWORD SetAddress：断点地址
int PointType：e(执行0)/w(写入1)/a(访问3)
int nLen：断点长度(1,2,4)
备注：(硬件断点如果是执行类型，可不指定断点长度)
*/
void CMyDbg::SetBpHard(DWORD SetAddress, int PointType, int nLen)
{
	//表示第几个被设置
	int DrTmp;

	if (m_HWflag.DR0 == FALSE)
	{
		DrTmp = 1;
		SetHardWarePoint(SetAddress, DrTmp, PointType, nLen);
		return;
	}
	else if (m_HWflag.DR1 == FALSE)
	{
		DrTmp = 2;
		SetHardWarePoint(SetAddress, DrTmp, PointType, nLen);
		return;
	}
	else if (m_HWflag.DR2 == FALSE)
	{
		DrTmp = 3;
		SetHardWarePoint(SetAddress, DrTmp, PointType, nLen);
		return;
	}
	else if (m_HWflag.DR3 == FALSE)
	{
		DrTmp = 4;
		SetHardWarePoint(SetAddress, DrTmp, PointType, nLen);
		return;
	}
	else
	{
		m_UI.Message("硬件断点无可用寄存器，请释放后再设置!\r\n");
	}
	return;
}

void CMyDbg::DebughardWareProc()
{
	//保存当前第几个寄存器
	int i_Sel;

	//释放设置的硬件标志
	if ((m_Context.Dr6 & 1) == 1)
	{
		m_HWBPointValue = m_Context.Dr7;
		//清除
		m_Context.Dr7 &= 0xfffffffe;
		m_HWHMFlag = TRUE;
		i_Sel = 1;
	}
	else if ((m_Context.Dr6 & 2) == 2)
	{
		m_HWBPointValue = m_Context.Dr7;
		m_Context.Dr7 &= 0xfffffffb;
		m_HWHMFlag = TRUE;
		i_Sel = 2;
	}
	else if ((m_Context.Dr6 & 4) == 4)
	{
		m_HWBPointValue = m_Context.Dr7;
		m_Context.Dr7 &= 0xffffffef;
		m_HWHMFlag = TRUE;
		i_Sel = 3;
	}
	else if ((m_Context.Dr6 & 8) == 8)
	{
		m_HWBPointValue = m_Context.Dr7;
		m_Context.Dr7 &= 0xffffffbf;
		m_HWHMFlag = TRUE;
		i_Sel = 4;
	}

	m_IsInput = TRUE;

	if (m_HWHMFlag == TRUE)
	{
		char szCurCode[32] = { 0 };
		//取指令
		ReadProcessMemory(m_hProcess,
			(LPVOID)m_Context.Eip,
			szCurCode,
			sizeof(szCurCode),
			NULL);
		m_CurCode.pCurCodeEntryAddress = m_Context.Eip;
		m_CurCode.szCurLineCode[0] = szCurCode[0];
		m_CurCode.szCurLineCode[1] = szCurCode[1];

		//触发了哪个寄存器的断点
		switch (i_Sel)
		{
		case 1:
			if (((m_Context.Dr7 >> 16) & 3) == 0)
			{
				printf("当前EIP：0x%08X 触发硬件执行断点\r\n", m_Context.Eip);
			}
			else if (((m_Context.Dr7 >> 16) & 3) == 1)
			{
				printf("当前EIP：0x%08X 触发硬件写断点\r\n", m_Context.Eip);
			}
			else if (((m_Context.Dr7 >> 16) & 3) == 3)
			{
				printf("当前EIP：0x%08X 触发硬件读或写断点\r\n", m_Context.Eip);
			}

			break;
		case 2:
			if (((m_Context.Dr7 >> 20) & 3) == 0)
			{
				printf("当前EIP：0x%08X 触发硬件执行断点\r\n", m_Context.Eip);
			}
			else if (((m_Context.Dr7 >> 20) & 3) == 1)
			{
				printf("当前EIP：0x%08X 触发硬件写断点\r\n", m_Context.Eip);
			}
			else if (((m_Context.Dr7 >> 20) & 3) == 3)
			{
				printf("当前EIP：0x%08X 触发硬件读或写断点\r\n", m_Context.Eip);
			}
			break;
		case 3:
			if (((m_Context.Dr7 >> 24) & 3) == 0)
			{
				printf("当前EIP：0x%08X 触发硬件执行断点\r\n", m_Context.Eip);
			}
			else if (((m_Context.Dr7 >> 24) & 3) == 1)
			{
				printf("当前EIP：0x%08X 触发硬件写断点\r\n", m_Context.Eip);
			}
			else if (((m_Context.Dr7 >> 24) & 3) == 3)
			{
				printf("当前EIP：0x%08X 触发硬件读或写断点\r\n", m_Context.Eip);
			}
			break;
		case 4:
			if (((m_Context.Dr7 >> 28) & 3) == 0)
			{
				printf("当前EIP：0x%08X 触发硬件执行断点\r\n", m_Context.Eip);
			}
			else if (((m_Context.Dr7 >> 28) & 3) == 1)
			{
				printf("当前EIP：0x%08X 触发硬件写断点\r\n", m_Context.Eip);
			}
			else if (((m_Context.Dr7 >> 28) & 3) == 3)
			{
				printf("当前EIP：0x%08X 触发硬件读或写断点\r\n", m_Context.Eip);
			}
			break;
		default:
			break;
		}

		m_IsInput = FALSE;

		//打印寄存器环境
		DisplayRegisterInfo();
		//显示汇编指令
		DisplayAssembly(szCurCode, m_CurCode.pCurCodeEntryAddress);
		//设置单步
		m_Context.EFlags |= 0x100;
		//硬件断点引起的单步重设标记
		m_EpnFlag.bHardWareSetinFlag = TRUE;
		//内存断点与硬件断点组合下断点时存在对应关系
		m_HWHMFlag = FALSE;
	}
}

/*
功能：删除硬件断点，释放设置的硬件标志
并恢复由硬件断点设置导致int3断点失效的断点
参数：
int nChoise：DR寄存器序号，DR0=>1,..,DR4=>4
*/
void CMyDbg::DelBpHard(int nChoise)
{
	switch (nChoise)
	{
		//DR0
	case 1:
		//清除
		m_Context.Dr7 &= 0xfffffffe;
		SetBpInt3Again(m_Context.Dr0);
		m_Context.Dr0 = 0;
		m_HWflag.DR0 = FALSE;
		break;
		//DR1
	case 2:
		m_Context.Dr7 &= 0xfffffffb;
		SetBpInt3Again(m_Context.Dr1);
		m_Context.Dr1 = 0;
		m_HWflag.DR1 = FALSE;
		break;
		//DR2
	case 3:
		m_Context.Dr7 &= 0xffffffef;
		SetBpInt3Again(m_Context.Dr2);
		m_Context.Dr2 = 0;
		m_HWflag.DR2 = FALSE;
		break;
		//DR3
	case 4:
		m_Context.Dr7 &= 0xffffffbf;
		SetBpInt3Again(m_Context.Dr3);
		m_Context.Dr3 = 0;
		m_HWflag.DR3 = FALSE;
		break;
	}
}

/*
功能：设置内存断点
参数：
DWORD dwAddr：地址
size_t nSize：内存断点内存长度
const char* pszType：r(读)/w(写)
备注：这里先不解决跨页内存断点
*/
void CMyDbg::SetBpMem(DWORD dwAddr, size_t nSize, const char* pszType)
{
	tagBpType bpMemType;
	//检测内存断点类型
	if (strcmp(pszType, "r") == 0)
	{
		m_bpMemFlag.bMemRead = TRUE;
		bpMemType = BP_MEM_READ;
		bMemRW = READ;
	}
	else if (strcmp(pszType, "w") == 0)
	{
		m_bpMemFlag.bMemWrite = TRUE;
		bpMemType = BP_MEM_WRITE;
		bMemRW = WRITE;
	}
	else {
		m_UI.Message("内存断点类型错误,请重新输入,[r(读)/w(写)]\r\n");
		return;
	}

	//设置内存断点
	//初始化存储结构
	tagBpMemInfor bpMemInfor;
	ZeroMemory(&m_bpMemPage, sizeof(tagBpMemPage));
	ZeroMemory(&bpMemInfor, sizeof(tagBpMemInfor));
	m_bpMemPage.m_pBpMemInfor = new std::list<tagBpMemInfor>;

	//触发EXCEPTION_ACCESS_VIOLATION异常
	if (TRUE == m_bpMemFlag.bMemRead)
	{
		bpMemType = BP_MEM_READ;
		bMemRW = READ;
		//内存读断点，设置为PAGE_NOACCESS
		VirtualProtectEx(m_hProcess,
			(void*)dwAddr,
			1,
			PAGE_NOACCESS,
			&m_bpMemPage.OldPageProtect);

		bpMemInfor.OldPageProtect = m_bpMemPage.OldPageProtect;

		//复位
		m_bpMemFlag.bMemRead = FALSE;
	}
	if (TRUE == m_bpMemFlag.bMemWrite)
	{
		bpMemType = BP_MEM_WRITE;
		bMemRW = WRITE;
		//内存写断点，设置为PAGE_EXECUTE_READ
		VirtualProtectEx(m_hProcess,
			(void*)dwAddr,
			1,
			PAGE_EXECUTE_READ,
			&m_bpMemPage.OldPageProtect);

		bpMemInfor.OldPageProtect = m_bpMemPage.OldPageProtect;

		//复位
		m_bpMemFlag.bMemWrite = FALSE;
	}

	//记录断点信息
	bpMemInfor.bDelFlag = TRUE;
	bpMemInfor.pBpType = bpMemType;
	bpMemInfor.dwAddr = dwAddr;
	bpMemInfor.nSize = nSize;
	m_pBpMemInforList.push_back(bpMemInfor);

	//记录到分页中
	//m_bpMemPage.m_pBpMemInfor->push_back(bpMemInfor);
	//m_pBpMemList.push_back(m_bpMemPage);

	return;
}

void CMyDbg::ShowBpMemList()
{
	std::list<tagBpMemInfor>::iterator iter;
	size_t i = 0;

	m_UI.Output("序号\t 地址\t 长度\t 类型\t\r\n");

	for (iter = m_pBpMemInforList.begin();
		iter != m_pBpMemInforList.end();
		iter++)
	{
		if (TRUE == iter->bDelFlag)
		{
			m_UI.Output("%2d\t 0x%08X %d ",
				i + 1,
				iter->dwAddr,
				iter->nSize);
			if (iter->pBpType == BP_MEM_READ)
				printf(" READ\r\n");
			if (iter->pBpType == BP_MEM_WRITE)
				printf(" WRITE\r\n");

			i++;
		}
	}
}

/*
功能：删除内存断点
参数：
size_t nIdx：序号
*/
void CMyDbg::DelBpMem(size_t nIdx)
{
	std::list<tagBpMemInfor>::iterator iter;
	size_t i = 0;
	for (iter = m_pBpMemInforList.begin();
		iter != m_pBpMemInforList.end();
		iter++)
	{
		if (TRUE == iter->bDelFlag && i + 1 == nIdx)
		{
			iter->bDelFlag = FALSE;
			i++;
		}
	}
}

void CMyDbg::SetMemPageAllAccess()
{
	if (m_pBpMemInforList.empty())
		return;

	DWORD dwProtect;
	VirtualProtectEx(m_hProcess,
		(void*)m_pBpMemInforList.begin()->dwAddr,
		1,
		m_pBpMemInforList.begin()->OldPageProtect,
		&dwProtect);
}

void CMyDbg::SetMemPageNoAccess()
{
	if (m_pBpMemInforList.empty())
		return;

	DWORD dwProtect;
	std::list<tagBpMemInfor>::iterator iter;
	for (iter = m_pBpMemInforList.begin();
		iter != m_pBpMemInforList.end();
		iter++)
	{

		if (iter->pBpType == BP_MEM_READ) {
			VirtualProtectEx(m_hProcess,
				(void*)iter->dwAddr,
				1,
				PAGE_NOACCESS,
				&dwProtect);
		}

		if (iter->pBpType == BP_MEM_WRITE) {
			VirtualProtectEx(m_hProcess,
				(void*)iter->dwAddr,
				1,
				PAGE_EXECUTE_READ,
				&dwProtect);
		}
	}
}

void CMyDbg::StartDllParse(LOAD_DLL_DEBUG_INFO dllDebugInfo)
{
	char   szDllNameTmp[24] = { 0 };
	char   szDllBuffer[1024] = { 0 };
	DWORD  dwOldProto = 0;
	LPVOID CurDllAddress = (LPVOID)dllDebugInfo.lpBaseOfDll;
	BOOL bRet = FALSE;
	//取得DLL文件名
	bRet = ReadProcessMemory(m_hProcess,
		(LPVOID)CurDllAddress,
		szDllBuffer,
		sizeof(szDllBuffer),
		&dwOldProto);
	if (!bRet)
		return;

	//检测DLL文件是否是有效PE文件
	if (!m_hFileInfor.IsPeFile(szDllBuffer))
		return;

	//解析导入表
	//m_hFileInfor.ParseImportTable(m_hProcess, CurDllAddress);
	//解析导出表
	m_hFileInfor.ParseExportTable(m_hProcess, szDllBuffer, CurDllAddress);
}

/*
功能：解析导出表，显示相应的DLL名和导出函数(名称或序号）
参数：
const char* pszBuf：相对地址或寄存器
比如：
- call dword ptr ds:[0x10011F4] 等价于
  call dword ptr ds:[<&msvcrt.__getmainargs>] ;  msvcrt.__getmainargs
- call 010127AA 等价于
  call <jmp.&msvcrt._initterm>

- jmp dword ptr ds:[0x10011BC] 等价于
  jmp dword ptr ds:[<&msvcrt.__CxxFrameHandler>] ;  msvcrt.__CxxFrameHandler3
- jmp 0101263C 等价于
  jmp <jmp.&msvcrt.__CxxFrameHandler>

- mov edi,dword ptr ds:[0x1001020] 等价于
  mov edi,dword ptr ds:[<&KERNEL32.GetModuleHandleA>] ; |kernel32.GetModuleHandleA
*/
void CMyDbg::ShowExportFunc(const char* pszBuf)
{
	int i;
	int n = strlen(pszBuf);      //汇编指令长度，如 call Dword ptr DS:[0100120C]
	char szBufTmp[16] = { 0 };
	char szAddr[32] = { 0 };
	memcpy(szBufTmp, pszBuf, 4); //截取OPCode

	//解析3个指令 转换相应函数
	while (strcmp(szBufTmp, "call") == 0 ||
		strcmp(szBufTmp, "jmp ") == 0 ||
		strcmp(szBufTmp, "mov ") == 0)
	{
		if (*pszBuf == NULL) //无效数据，直接返回
			return;

		for (i = 3; i < n; i++) //遍历OPCode之后的内容
		{
			//匹配到此种形式，比如：call Dword ptr DS:[0x10011F4]
			if ((pszBuf[i] == '[') && (pszBuf[i + 9] == ']'))
			{
				char szBuffer[9] = { 0 };     //跳转地址
				char szTmp[8] = { 0 };        //OPCode
				DWORD dwOldProto = 0;         //旧的内存属性
				DWORD dwAddr = 0;              //要跳转的地址
				unsigned int nFunAddrOffset = 0;

				char szJumpCode[256] = { 0 }; //得到反汇编指令信息
				UINT nCodeSize = 0;           //解析指令长度
				char szJmpBuff[64] = { 0 };   //要跳转到的位置处指令

				strncpy(szBuffer, (const char*)& pszBuf[i + 1], 8); //取得跳转地址，比如0x10011F4
				StrConvDec(szBuffer, (unsigned int&)dwAddr);        //转换字符串类型地址为数值表示的地址
				if (0 == dwAddr) //无效地址，直接返回
					return;

				//取得要跳转到的地址位置处的指令信息
				//比如：01001048 > .  70173376               dd kernel32.GetStartupInfoA
				ReadProcessMemory(m_hProcess,
					(LPVOID)dwAddr,
					&szJmpBuff,
					sizeof(szJmpBuff),
					&dwOldProto);
				//反汇编
				Decode2Asm((PBYTE)szJmpBuff,
					szJumpCode,
					&nCodeSize,
					m_CurCode.pCurCodeEntryAddress);
				//保存OPCode
				memcpy(szTmp, szJumpCode, 4);
				//内层出现递归调用的情况，则继续向下内层查找
				if ((strcmp(szTmp, "call") == 0) || //
					(strcmp(szTmp, "jmp ") == 0) || //
					(strcmp(szBufTmp, "mov ") == 0))
				{
					int j = strlen(szJumpCode); //遍历内层OPCode之后的汇编指令字符串
					for (i = 3; i < j; i++)
					{
						//匹配到跳转，继续向下内层查找
						if ((szJumpCode[i] == '[') && (szJumpCode[i + 9] == ']') ||
							(szJumpCode[i] == ' ') && (szJumpCode[i + 9] == NULL))
						{
							//递归调用
							ShowExportFunc(szJumpCode);
						}
					}
					//读到的就是函数地址
					ReadProcessMemory(m_hProcess,
						(LPVOID)dwAddr,
						&nFunAddrOffset,
						4,
						&dwOldProto);
					//根据函数地址找DLL名和函数名称
					FindFuncNameByAddrInExport(nFunAddrOffset);
					return;
				}
				else
				{
					ReadProcessMemory(m_hProcess,
						(LPVOID)dwAddr,
						&nFunAddrOffset,
						4,
						&dwOldProto);
					FindFuncNameByAddrInExport(nFunAddrOffset);
					return;
				}
				return;
			}

			//字面含义同上面的解释，可参考上面的注释
			//匹配到些种形式，比如：call 010127AA
			if ((pszBuf[i] == ' ') && (pszBuf[i + 9] == NULL))
			{
				char szBuffer[9] = { 0 };
				char szBuffer2[256] = { 0 };
				char szJumpCode[64] = { 0 };
				char szTmp[8] = { 0 };
				DWORD dwOldProto = 0;
				DWORD dwAddr = 0;              //要跳转的地址
				UINT nCodeSize = 0;
				unsigned int nFunAddrOffset = 0;
				strncpy(szBuffer, (const char*)& pszBuf[i + 1], 8);
				//解析寄存器的调用
				if (strcmp(szBuffer, "eax") == 0)
				{
					FindFuncNameByAddrInExport(m_Context.Eax);
					return;
				}
				if (strcmp(szBuffer, "ebx") == 0)
				{
					FindFuncNameByAddrInExport(m_Context.Ebx);
					return;
				}
				if (strcmp(szBuffer, "ecx") == 0)
				{
					FindFuncNameByAddrInExport(m_Context.Ecx);
					return;
				}
				if (strcmp(szBuffer, "edx") == 0)
				{
					FindFuncNameByAddrInExport(m_Context.Edx);
					return;
				}
				if (strcmp(szBuffer, "esi") == 0)
				{
					FindFuncNameByAddrInExport(m_Context.Esi);
					return;
				}
				if (strcmp(szBuffer, "edi") == 0)
				{
					FindFuncNameByAddrInExport(m_Context.Edi);
					return;
				}
				StrConvDec(szBuffer, (unsigned int&)dwAddr);
				if (0 == dwAddr)
					return;

				ReadProcessMemory(m_hProcess,
					(LPVOID)dwAddr,
					&szBuffer2,
					sizeof(szBuffer2),
					&dwOldProto);
				Decode2Asm((PBYTE)szBuffer2,
					szJumpCode,
					&nCodeSize,
					m_CurCode.pCurCodeEntryAddress);
				memcpy(szTmp, szJumpCode, 4);
				if ((strcmp(szTmp, "call") == 0) ||
					(strcmp(szTmp, "jmp ") == 0) ||
					(strcmp(szBufTmp, "mov ") == 0))
				{
					int j = strlen(szJumpCode);
					for (i = 3; i < j; i++)
					{
						if ((szJumpCode[i] == '[') && (szJumpCode[i + 9] == ']') ||
							(szJumpCode[i] == ' ') && (szJumpCode[i + 9] == NULL))
						{
							ShowExportFunc(szJumpCode);
						}
					}
					ReadProcessMemory(m_hProcess,
						(LPVOID)dwAddr,
						&nFunAddrOffset,
						4,
						&dwOldProto);
					FindFuncNameByAddrInExport(nFunAddrOffset);
					return;
				}
				else
				{
					ReadProcessMemory(m_hProcess,
						(LPVOID)dwAddr,
						&nFunAddrOffset,
						4,
						&dwOldProto);
					FindFuncNameByAddrInExport(nFunAddrOffset);
					return;
				}
				return;
			}
		}
		if (i == n)
			break;
	}
}

BOOL CMyDbg::FindFuncNameByAddrInExport(DWORD dwAddr)
{
	if (dwAddr == 0) //无效地址，直接返回
		return FALSE;

	std::list<ExportTableInfor>::iterator iter; //外层
	std::list<ExportFunInfor>::iterator it;//内层

	//遍历外层导出表
	for (iter = m_hFileInfor.m_ExportTableList.begin();
		iter != m_hFileInfor.m_ExportTableList.end();
		iter++)
	{
		//遍历内层内部导出表结构
		for (it = iter->pEptTableFun->begin();
			it != iter->pEptTableFun->end();
			it++)
		{
			if (it->dwFunAddress == dwAddr)
			{
				if (it->szFunName == NULL)
				{
					m_UI.Output("\t\t\t\t\t%s.#:%d\r\n", iter->szDllTableName, it->nExportNumber);
				}
				else
				{
					m_UI.Output("\t\t\t\t\t%s.%s\r\n", iter->szDllTableName, it->szFunName);
				}
				return TRUE;
			}
		}
	}

	return FALSE;
}

/*
功能：导入一个脚本文件（后缀名为SCP）。会逐行解析脚本文件的命令，并执行该命令。
*/
void CMyDbg::LoadScript()
{
	char szFileName[MAX_PATH] = "";
	OPENFILENAME ofn;
	HANDLE hFile;
	HANDLE hFileMapHandle;
	LPVOID hFileMapImageBase;

	//打开要导入的脚本文件
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "scp Files(*.scp)\0*.scp\0All Files(*.*)\0*.*\0\0";
	ofn.nFilterIndex = 1;
	if (!GetOpenFileName(&ofn))
	{
		m_UI.Message("GetOpenFileName False， err:%d\r\n", GetLastError());
		return;
	}

	//打开文件
	hFile = CreateFile(szFileName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_UI.Message("文件打开失败， err:%d\r\n", GetLastError());
		return;
	}

	//建立文件映射
	hFileMapHandle = ::CreateFileMapping(hFile,
		NULL,
		PAGE_READWRITE,
		NULL,
		NULL,
		NULL);
	if (hFileMapHandle == NULL)
	{
		m_UI.Message("建立文件映射失败， err:%d\r\n", GetLastError());
		CloseHandle(hFile);
		return;
	}

	//文件映射到内存
	hFileMapImageBase = ::MapViewOfFile(hFileMapHandle,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);

	//获得文件的基地址 
	if (hFileMapImageBase == NULL)
	{
		m_UI.Message("文件映射失败， err:%d\r\n", GetLastError());
		CloseHandle(hFileMapHandle);
		CloseHandle(hFile);
		return;
	}
	//处理命令
	HandlerLoadScript((const char*)hFileMapImageBase);

	//卸载内存映射
	if (hFileMapImageBase)
	{
		UnmapViewOfFile(hFileMapImageBase);
	}
	//关闭内存映射句柄
	if (hFileMapHandle)
	{
		CloseHandle(hFileMapHandle);
	}
	//关闭文件句柄
	if (hFile)
	{
		CloseHandle(hFile);
	}
	return;
}

/*
功能：处理文件中的命令
参数：
const char* pFileMapImage：文件映射到内存中的内存首地址
*/
void CMyDbg::HandlerLoadScript(const char* pFileMapImage)
{
	char* pStr = (char*)pFileMapImage;
	int i = 0, j = 0;
	char strCmdTmp[256] = { 0 };
	char strCmd[256] = { 0 };

	//开始读取并解析指令
	while (*(pStr + i) != '\0')
	{
		//解析换行，每命令为一行
		if (*(pStr + i) == '\r' && *(pStr + i + 1) == '\n')
		{
			//拷贝命令
			strcpy(strCmd, strCmdTmp);
			//自动导入标志置位
			bSelIptFlag = TRUE;
			//解析命令
			m_cmdIptList.push_back(strCmd);
			//清理缓冲区
			memset(strCmdTmp, 0, sizeof(strCmdTmp));
			memset(strCmd, 0, sizeof(strCmd));
			//复位外层命令缓冲区
			j = 0;
			//忽略\r\n
			i += 2;
		}
		//记录命令
		strCmdTmp[j] = *(pStr + i);
		//向后移动
		i++;
		j++;
	}
	m_UI.Output("脚本导入完成\r\n");
}

/*
功能：将用户操作的所有有效命令导出到一个脚本文件。
*/
void CMyDbg::ExportScript()
{
	m_UI.Output("脚本正在导出...\r\n");
	int nCount, j = 0;
	DWORD dwOut;

	//获取当前调试进程的文件名
	nCount = j = strlen(m_hFileInfor.m_CPathFileName); //当前打开的文件全路径
	char* pStr = m_hFileInfor.m_CPathFileName; //指向当前打开的文件
	char szCmdTmp[MAX_PATH] = { 0 };    //命令
	char szFileName[MAX_PATH] = { 0 };  //要导出的文件名

	/*
	反向遍历查找文件名之前的全路径，
	比如C:\\Windows\\System32\\kernel32.dll,
	返回结果C:\\Windows\\System32\\
	返回位置
	*/
	j -= 1;
	while (*(pStr + j) != '\\')
	{
		j--;
	}
	memcpy(szFileName, (LPVOID)pStr, j + 1);
	//提取文件名，C:\\Windows\\System32\\kernel32
	memcpy(szCmdTmp, (LPVOID)(pStr + j + 1), nCount - j);
	j = 0;
	while (szCmdTmp[j] != '.')
	{
		j++;
	}
	//添加脚本导出后缀.SCP
	strncat(szFileName, szCmdTmp, j);
	strcat(szFileName, ".SCP");
	szFileName[MAX_PATH - 1] = '\0';
	//创建文件
	HANDLE hfile = CreateFile(szFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//执行导出
	memset(szCmdTmp, 0, sizeof(szCmdTmp));
	std::list<std::string>::iterator iter;
	//遍历历史输入命令
	for (iter = m_cmdRecordList.begin();
		iter != m_cmdRecordList.end();
		iter++)
	{
		//按行组织准备导出的命令
		memset(szCmdTmp, 0, sizeof(szCmdTmp));
		strcpy(szCmdTmp, iter->data());
		strcat(szCmdTmp, "\r\n");
		//将命令写入到导出脚本文件
		WriteFile(hfile,
			szCmdTmp,
			strlen(szCmdTmp),
			&dwOut,
			NULL);
	}

	//已经手动导出
	bEptScriptFlag = TRUE;
	//关闭文件
	CloseHandle(hfile);

	m_UI.Output("脚本导出完成。\r\n");
	m_UI.Output("%s \r\n", szFileName);
}

void CMyDbg::ShowModuleList()
{
	int i = 0;

	std::list<ExportTableInfor>::iterator iter; //外层
	std::list<ExportFunInfor>::iterator it;//内层

	//遍历外层导出表
	for (iter = m_hFileInfor.m_ExportTableList.begin();
		iter != m_hFileInfor.m_ExportTableList.end();
		iter++)
	{
		m_UI.Output("%d. %s.dll\r\n", i + 1, iter->szDllTableName);

		i++;
	}
}

DWORD CMyDbg::OnCreateThread()
{
	return DBG_CONTINUE;
}

DWORD CMyDbg::OnCreateProcess()
{
	CREATE_PROCESS_DEBUG_INFO info = m_Event.u.CreateProcessInfo;
	//保存进程信息
	m_DbgProcessInfo = info;
	m_hProcess = info.hProcess;
	//保存代码信息
	m_CurCode.pCurCodeEntryAddress = (DWORD)m_DbgProcessInfo.lpStartAddress;
	m_CurCode.pStartAddress = (DWORD)m_DbgProcessInfo.lpStartAddress;
	//入口断点，设置0xCC
	m_oepCode = SetBreakPoint(m_DbgProcessInfo.hProcess,
		m_DbgProcessInfo.lpStartAddress,
		_0xCC);
	m_UI.Output("OnCreateProcess StartAddress=%p\n",
		info.lpStartAddress);

	return DBG_CONTINUE;
}

DWORD CMyDbg::OnExitThread()
{
	return DBG_CONTINUE;
}

DWORD CMyDbg::OnExitProcess()
{
	return DBG_CONTINUE;
}

//解析加载的DLL，（导入表，导出表）
DWORD CMyDbg::OnLoadDll()
{
	LOAD_DLL_DEBUG_INFO info = m_Event.u.LoadDll;
	StartDllParse(info);

	LPLOAD_DLL_DEBUG_INFO pLoadDllDi = &m_Event.u.LoadDll;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Event.dwProcessId);
	wchar_t szImageNameU[MAX_PATH] = { 0 };
	char szImageNameA[MAX_PATH] = { 0 };

	//获取字符串的地址
	DWORD dwAddr = 0;
	DWORD dwBytesToRead = 0;
	ReadProcessMemory(hProcess, pLoadDllDi->lpImageName, &dwAddr, sizeof(dwAddr), &dwBytesToRead);

	//读取字符串
	if (pLoadDllDi->fUnicode) //unicode
	{
		ReadProcessMemory(hProcess,
			(LPVOID)dwAddr,
			szImageNameU,
			sizeof(szImageNameU),
			&dwBytesToRead);

		wprintf(L"%s \r\n", szImageNameU);
	}
	else //ascii
	{

		ReadProcessMemory(hProcess,
			(LPVOID)dwAddr,
			szImageNameA,
			sizeof(szImageNameA),
			&dwBytesToRead);

		printf("%s \r\n", szImageNameA);
	}

	//static unsigned int nLoadDllCount = 0;
	//if (m_bFirstLoadDll == TRUE)
	//{
	//	LOAD_DLL_DEBUG_INFO info = m_Event.u.LoadDll;
	//	if (!nLoadDllCount)
	//	{
	//		printf("Please wait a moment ...\r\n");
	//		nLoadDllCount++;
	//	}

	//	StartDllParse(info);
	//}

	return DBG_CONTINUE;
}

DWORD CMyDbg::OnUnLoadDll()
{
	return DBG_CONTINUE;
}

DWORD CMyDbg::OnOutputDebugString()
{
	return DBG_CONTINUE;
}
