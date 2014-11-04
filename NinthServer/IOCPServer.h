// IOCPServer.h: interface for the CIOCPServer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <winsock2.h>
#include <MSTcpIP.h>
#pragma comment(lib,"ws2_32.lib")
#include "Buffer.h"
#include <process.h>
#include <afxtempl.h>
#include "zlib/zlib.h"
#pragma comment(lib,"zlib/zlib.lib")

enum{
	MAX_BUFFER_LEN		= 8192,
	HDR_SIZE			= 12,
	FLAG_SIZE			= 4,
	HUERISTIC_VALUE		= 2
};
enum{
	NC_CLIENT_CONNECT		=	0x0001,
	NC_CLIENT_DISCONNECT	=	0x0002,
	NC_TRANSMIT				=	0x0003,
	NC_RECEIVE				=	0x0004,
	NC_RECEIVE_COMPLETE		=	0x0005 
};


class CLock
{
public:
	CLock(CRITICAL_SECTION& cs, const CString& strFunc)
	{
		m_strFunc = strFunc;
		m_pcs = &cs;
		Lock();
	}
	~CLock()
	{
		Unlock();
	}
	void Unlock()
	{
		LeaveCriticalSection(m_pcs);
	}
	void Lock()
	{
		EnterCriticalSection(m_pcs);
	}
protected:
	CRITICAL_SECTION*	m_pcs;
	CString				m_strFunc;
};

enum IOType 
{
	IOInitialize,
	IORead,
	IOWrite,
	IOIdle
};


class OVERLAPPEDPLUS 
{
public:
	OVERLAPPED			m_ol;
	IOType				m_ioType;

	OVERLAPPEDPLUS(IOType ioType) {
		ZeroMemory(this, sizeof(OVERLAPPEDPLUS));
		m_ioType = ioType;
	}
};


struct ClientContext
{
	SOCKET				m_Socket;
	// Store buffers
	CBuffer				m_WriteBuffer;
	CBuffer				m_CompressionBuffer;	// 接收到的压缩的数据
	CBuffer				m_DeCompressionBuffer;	// 解压后的数据
	CBuffer				m_ResendWriteBuffer;	// 上次发送的数据包，接收失败时重发时用
	//int					m_Dialog[2]; // 放对话框列表用，第一个int是类型，第二个是CDialog的地址
	int					m_nTransferProgress;
	// Input Elements for Winsock
	WSABUF				m_wsaInBuffer;
	BYTE				m_byInBuffer[8192];
	// Output elements for Winsock
	WSABUF				m_wsaOutBuffer;
	HANDLE				m_hWriteComplete;
	BOOL				m_bIsMainSocket; // 是不是主socket
	UINT				m_ID;
	ClientContext()
	{
		m_ID	 = NULL;
		m_Socket = INVALID_SOCKET;
	}
};
class CNinthServerDlg;

typedef void (CALLBACK* NOTIFYPROC)(LPVOID, ClientContext*, UINT nCode);
typedef CList<ClientContext*, ClientContext* > ContextList;

class CIOCPServer
{
private:
	
	LONG					m_nWorkerCnt;

	bool					m_bInit;
	bool					m_bDisconnectAll;
	BYTE					m_bPacketFlag[4];
	ContextList				m_listContexts;
	ContextList				m_listFreePool;
	WSAEVENT				m_hEvent;
	
	HANDLE					m_hKillEvent;
	HANDLE					m_hThread;
	HANDLE					m_hCompletionPort;
	bool					m_bTimeToKill;

	LONG					m_nKeepLiveTime; // 心跳超时

	// Thread Pool 
	// 	LONG					m_nThreadPoolMin;
	// 	LONG					m_nThreadPoolMax;
	LONG					m_nThreadPoolNumber;
	//LONG					m_nCurrentThreads;
	//LONG					m_nBusyThreads;
	
	static CRITICAL_SECTION	m_cs;
public:
	CNinthServerDlg*        m_pDialog;
	NOTIFYPROC				m_pNotifyProc;
	SOCKET					m_socListen;    
	UINT					m_nMaxConnections; // 最大连接数
	void DisconnectAll();
	CIOCPServer();
	virtual ~CIOCPServer();
	bool Initialize(NOTIFYPROC pNotifyProc,CNinthServerDlg* pDlg,int nMaxConnections, int nPort);
	static unsigned __stdcall ListenThreadProc(LPVOID lpVoid);
	static unsigned __stdcall ThreadPoolFunc(LPVOID WorkContext);
	void Send(ClientContext* pContext, LPBYTE lpData, UINT nSize);
	bool IsRunning();
	void Shutdown();
	void ResetConnection(ClientContext* pContext);

protected:
	void PostRecv(ClientContext* pContext);
	void InitializeClientRead(ClientContext* pContext);
	BOOL AssociateSocketWithCompletionPort(SOCKET device, HANDLE hCompletionPort, DWORD dwCompletionKey);
	void RemoveStaleClient(ClientContext* pContext, BOOL bGraceful);
	void MoveToFreePool(ClientContext *pContext);
	ClientContext*  AllocateContext();
	void CloseCompletionPort();
	void OnAccept();
	bool InitializeIOCP(void);
	void Stop();

	//CString GetHostName(SOCKET socket);

	void CreateStream(ClientContext* pContext);

	void ProcessIOMessage(IOType iotype,ClientContext *pClientContext,DWORD dwIoSize);
	bool OnClientInitializing	(ClientContext* pContext, DWORD dwSize = 0);
	bool OnClientReading		(ClientContext* pContext, DWORD dwSize = 0);
	bool OnClientWriting		(ClientContext* pContext, DWORD dwSize = 0);

};
