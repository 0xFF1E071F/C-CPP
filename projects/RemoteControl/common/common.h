#pragma once

#ifndef _WINDOWS_H
#define _WINDOWS_H
#include <Windows.h>
#endif // !_WINDOWS_H

//packet:
// 1. 类型标识
// 2. 数据长度
// 3. 数据

//消息类型
enum { 
    CLIENT_PUBLIC = 1,   //client -> server   客户端发往服务端
    SERVER_PUBLIC_REPLY, //server -> client   服务端发往客户端
    MSG_HEARTBEAT,    //心跳
    MSG_SCREEN,       //截屏
    MSG_SCREEN_CLOSE, //结束发送截屏数据
    MSG_FILE,         //文件
    MSG_CMD,          //CMD
    MSG_CMD_STOP,     //CMD标记此次发送结束
    MSG_CMD_YES,      //CMD回应客户端，不要在发送通知
    MSG_PROCESS,       //进程
    MSG_PROCESS_TERMINATE //结束进程
};

#define T1 60 //idle time before heartbeat
#define T2 10 //time to wait for response

#pragma pack(push, 1)
struct tagPacket {
    BYTE code;
    size_t length;
    //char szData[0]; //柔性数组
};
#pragma pack(pop)


//收取指定长度的数据
inline bool RecvData(SOCKET s, char* pBuf, int nLength)
{
    int nCurSize = 0;
    int nTotalSize = 0;
    while (nTotalSize < nLength) {

        nCurSize = recv(s,
            (char*)pBuf + nTotalSize,
            nLength - nTotalSize,
            0);
        if (nCurSize == 0 || nCurSize == SOCKET_ERROR) {
            //tcp连接关闭了，优雅断开
            return false;
        }
        else {
            nTotalSize += nCurSize;
        }
    }

    return true;
}

//发送指定的数据，并封装成packet
inline bool SendData(SOCKET s, int nType,const char* pBuf, int nLength)
{
    char* pSendBuf = new char[sizeof(tagPacket) + nLength];
    if (pSendBuf == NULL){
        return false;
    }


    tagPacket* pkt = (tagPacket*)pSendBuf;
    pkt->code = nType;
    pkt->length = nLength;

    if (nLength > 0){

        memcpy(pSendBuf + sizeof(tagPacket) , pBuf,nLength);
    }

    //需要循环的发送
    send(s,
         (char*)pSendBuf,
         sizeof(tagPacket) + pkt->length,
         0);

    if (pSendBuf != NULL){
        delete [] pSendBuf;
        pSendBuf = NULL;
    }

    return true;
}