#pragma once

#ifndef _WINDOWS_H
#define _WINDOWS_H
#include <Windows.h>
#endif

class CTcpSocket;

class DskCapture
{

public:
    explicit DskCapture(SOCKET sock);

	bool ScreenCapture();     //桌面截屏
	bool PaintMouse(HDC hdc); //绘制鼠标

private:
	SOCKET m_sock;
};