#pragma once
#include "afxwin.h"

#include "IOCPServer.h"
#include <memory>
using std::auto_ptr;
// CCmdDialog �Ի���

class CCmdDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCmdDialog)

public:
	CCmdDialog(CIOCPServer* pServer,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCmdDialog();

// �Ի�������
	enum { IDD = IDD_CMD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	CEdit m_cmdEdit;
	CIOCPServer*		m_iocpServer;
	ClientContext*		m_Context;
	UINT				m_nReceiveLength;
	UINT				m_nCurSel;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	void Start(CIOCPServer* pServer,void* pClient);
	void OnReceiveComplete(ClientContext *pContext);
	void AddKeyBoardData(ClientContext *pContext);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnEnChangeEdit();
};
