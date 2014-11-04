
// NinthServerDlg.h : ͷ�ļ�
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
// CNinthServerDlg �Ի���
class CNinthServerDlg : public CDialogEx
{
// ����
public:
	CNinthServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NINTHSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
