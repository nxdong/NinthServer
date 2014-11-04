// CmdDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NinthServer.h"
#include "CmdDialog.h"
#include "afxdialogex.h"


// CCmdDialog �Ի���

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


// CCmdDialog ��Ϣ�������


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
	// TODO: �ڴ˴������Ϣ����������
}


BOOL CCmdDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_iocpServer = NULL;
	m_nCurSel = m_edit.GetWindowTextLength();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CCmdDialog::Start(CIOCPServer* dwServer,void* pClient)
{
	m_iocpServer    = dwServer;
	m_Context		= (ClientContext*)pClient;
	// ֪ͨԶ�̿��ƶ˶Ի����Ѿ���
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
	// �������0

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
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		// ����VK_ESCAPE��VK_DELETE
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
		// ����VK_BACK
		if (pMsg->wParam == VK_BACK && pMsg->hwnd == m_edit.m_hWnd)
		{
			if (m_cmdEdit.GetWindowTextLength() <= m_nReceiveLength)
				return true;
		}
	}
	// Ctrlû����
	if (pMsg->message == WM_CHAR && GetKeyState(VK_CONTROL) >= 0)
	{
		int	len = m_edit.GetWindowTextLength();
		m_edit.SetSel(len, len);
		// �û�ɾ���˲������ݣ��ı�m_nCurSel
		if (len < m_nCurSel)
			m_nCurSel = len;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCmdDialog::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	closesocket(m_Context->m_Socket);	
	CDialog::OnClose();

	CDialogEx::OnClose();
}


void CCmdDialog::OnEnChangeEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	int len = m_edit.GetWindowTextLength();
	if (len < m_nCurSel)
		m_nCurSel = len;

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
