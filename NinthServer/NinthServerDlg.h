
// NinthServerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "IOCPServer.h"
#include "ClientDlg.h"
#include <LM.h>
#include <map>
#include <utility>
using	std::make_pair;	
using	std::map;
typedef map<UINT,CClientDlg*> ClientMap; 
// CNinthServerDlg 对话框
class CNinthServerDlg : public CDialogEx
{
// 构造
public:
	CNinthServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NINTHSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl	m_listCtrl;
	int			m_Column_Count;
	static CIOCPServer*	m_iocpServer;
	ClientMap	m_clientMap;

public:
	void InitListCtrl();
	void StartIOCPServer();
	static  void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	void ProcessReceiveComplete(ClientContext *pContext);
	void ProcessReceive(ClientContext *pContext);
	void OnDisconnect(ClientContext* pContext);
	int	 AddToList(ClientContext* pContext);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnListManager();
};
