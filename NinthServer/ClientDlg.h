#pragma once
#include "afxcmn.h"
#include "IOCPServer.h"
#include "CmdDialog.h"
#include "FileDlg.h"
// CClientDlg �Ի���

class CClientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClientDlg)

public:
	CClientDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientDlg();

// �Ի�������
	enum { IDD = IDD_CLIENTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCtrl;
	ClientContext*			m_mainClientContext;
	CIOCPServer*			m_piocpServer;
	static CCmdDialog*			m_cmdDlg;
	static CFileDlg*			m_fileDlg;
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void	SetIOCPServerAndContext(CIOCPServer* dwServer,DWORD dwMainClientContxt);
	void	SendCmdCommand();
	void	SendFileCommand();
	void	OnReceiveComplete(ClientContext* pContext);

};
