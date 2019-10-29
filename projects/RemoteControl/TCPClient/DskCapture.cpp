#include "../../common/common.h"
#include "DskCapture.h"
#include "CRtlCompress.h"
#include <fstream>

DskCapture::DskCapture(SOCKET sock)
	: m_sock(sock)
{
	
}

bool DskCapture::ScreenCapture()
{
	HWND hDesktopWnd = NULL;
	HDC hDskDC = NULL;
	int dwScreenWidth = 0;
	int dwScreenHeight = 0;
	HBITMAP hBitmap = NULL;
	BOOL bRet = 0;

	// 获取桌面窗口句柄
	hDesktopWnd = GetDesktopWindow();
	// 获取桌面窗口DC
	hDskDC = GetDC(hDesktopWnd);
	// 创建兼容DC
	HDC hMemDC = CreateCompatibleDC(hDskDC);
	if (hMemDC == nullptr)
	{
		ReleaseDC(hDesktopWnd, hDskDC);
		return false;
	}
	// 获取计算机屏幕的宽和高
	dwScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	dwScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	// 创建兼容位图
	hBitmap = CreateCompatibleBitmap(hDskDC, dwScreenWidth, dwScreenHeight);
	// 选中位图
	SelectObject(hMemDC, hBitmap);
	// 将窗口内容绘制到位图上
	bRet = BitBlt(hMemDC, 0, 0, dwScreenWidth, dwScreenHeight, hDskDC, 0, 0, SRCCOPY);
	if (!bRet)
	{
		DeleteDC(hMemDC);
		ReleaseDC(hDesktopWnd, hDskDC);
		return false;
	}

	// 绘制鼠标
	PaintMouse(hMemDC);

	//位图所占内存大小(宽 x 高 x 4[ARGB])
	int nSize = dwScreenWidth * dwScreenHeight * 4;
	//缓冲区（位图宽度，位图高度，位图数据）
	char *pBuf = new char[nSize + 8]; //8,int类型的宽度和int类型的高度占8字节
	//写入位图的宽和高
	*(int*)pBuf = dwScreenWidth;
	*((int*)pBuf + 1) = dwScreenHeight;
	//写入位图数据
	GetBitmapBits(hBitmap, nSize, pBuf + 8);//+8，移动缓冲区指针到位图数据区
	// 发送到服务端
	
	CRtlCompress RtlCompress;
	//发送数据前压缩
	BYTE *pCompressData = NULL;     //压缩后数据
	DWORD dwCompressDataLength = 0; //压缩后数据大小
	bRet = RtlCompress.CompressData((BYTE*)pBuf, nSize+8, &pCompressData, &dwCompressDataLength);
	if (!bRet)
	{
		OutputDebugStringA("CompressData Failed!");
	}
	//发送压缩后的数据
	//m_pTcpSocket->SendBitmap(pBuf, nSize+8);
	//m_pTcpSocket->SendBitmap((char*)pCompressData, dwCompressDataLength);
	std::ofstream ofile("pCompressData.txt");
	if (!ofile)
	{
		return false;
	}
	ofile.write((const char*)pCompressData, dwCompressDataLength);
	ofile.close();

	SendData(m_sock, MSG_SCREEN, (const char*)pCompressData, dwCompressDataLength);

	// 释放资源
	if (pBuf != NULL)
	{
		delete[] pBuf;
	}

	if (pCompressData)
	{
		delete[]pCompressData;
		pCompressData = NULL;
	}

	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(hDesktopWnd, hDskDC);

	return true;
}

bool DskCapture::PaintMouse(HDC hdc)
{
	HDC hBufDC = NULL;
	ICONINFO iconInfo = { 0 };
	CURSORINFO cursorInfo = { 0 };
	HBITMAP hBmpOldMask = NULL;

	hBufDC = CreateCompatibleDC(hdc);
	RtlZeroMemory(&iconInfo, sizeof(iconInfo));
	cursorInfo.cbSize = sizeof(cursorInfo);
	// 获取光标信息
	GetCursorInfo(&cursorInfo);
	// 获取光标的图标信息
	GetIconInfo(cursorInfo.hCursor, &iconInfo);
	// 绘制白底黑鼠标(AND)
	hBmpOldMask = (HBITMAP)SelectObject(hBufDC, iconInfo.hbmMask);
	BitBlt(hdc, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y, 20, 20, hBufDC, 0, 0, SRCAND);
	// 绘制黑底彩色鼠标(OR)
	SelectObject(hBufDC, iconInfo.hbmColor);
	BitBlt(hdc, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y, 20, 20, hBufDC, 0, 0, SRCPAINT);

	// 释放资源
	SelectObject(hBufDC, hBmpOldMask);
	DeleteObject(iconInfo.hbmColor);
	DeleteObject(iconInfo.hbmMask);
	DeleteDC(hBufDC);

	return true;
}
