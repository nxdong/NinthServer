// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NinthServer.h"
#include "ClientDlg.h"
#include "afxdialogex.h"


// CClientDlg 对话框
CCmdDialog* CClientDlg::m_cmdDlg		= NULL;
CFileDlg*	CClientDlg::m_fileDlg	= NULL;

IMPLEMENT_DYNAMIC(CClientDlg, CDialogEx)

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
{

}

CClientDlg::~CClientDlg()
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CClientDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CClientDlg 消息处理程序


void CClientDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_tabCtrl.GetCurSel())
	{
	case 0:
		m_cmdDlg->ShowWindow(TRUE);
		m_fileDlg->ShowWindow(FALSE);
		break;
	case 1:
		m_cmdDlg->ShowWindow(FALSE);
		m_fileDlg->ShowWindow(TRUE);
		break;
	default:
		break;
	}
	*pResult = 0;
}


BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect tabRect;
	m_tabCtrl.InsertItem(0,_T("Command"));
	m_tabCtrl.InsertItem(1,_T("File"));
	m_tabCtrl.InsertItem(2,_T("Process"));
	m_tabCtrl.InsertItem(3,_T("Service"));
	m_cmdDlg = new CCmdDialog(m_piocpServer);
	m_fileDlg = new CFileDlg(m_piocpServer);
	m_cmdDlg->Create(IDD_CMD,&m_tabCtrl);
	m_fileDlg->Create(IDD_FILE,&m_tabCtrl);

	m_tabCtrl.GetClientRect(&tabRect);
	tabRect.left += 2;                  
	tabRect.right -= 2;   
	tabRect.top += 22;   
	tabRect.bottom -= 2; 
	m_cmdDlg->SetWindowPos(NULL,tabRect.left, tabRect.top,
		tabRect.Width(), tabRect.Height(),SWP_SHOWWINDOW);
	m_fileDlg->SetWindowPos(NULL,tabRect.left, tabRect.top,
		tabRect.Width(), tabRect.Height(),SWP_HIDEWINDOW);



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_tabCtrl.m_hWnd == NULL)
		return;      // Return if window is not created yet.
	CRect mainDlgRect;
	GetClientRect(&mainDlgRect);
	m_tabCtrl.MoveWindow(&mainDlgRect);   
	CRect tabRect;
	m_tabCtrl.GetClientRect(&tabRect);
	tabRect.left += 2;                  
	tabRect.right -= 2;   
	tabRect.top += 22;   
	tabRect.bottom -= 2; 

	m_cmdDlg->MoveWindow(tabRect);
	m_fileDlg->MoveWindow(tabRect);
}
void CClientDlg::SendCmdCommand()
{
	HEADER *cmd = new HEADER;
	cmd->flag = SERVER_SHELLMANAGER;
	m_piocpServer->Send(m_mainClientContext, (PBYTE)cmd, sizeof(HEADER));
}

void CClientDlg::SendFileCommand()
{
	HEADER *cmd = new HEADER;
	cmd->flag = SERVER_FILEMANAGER;
	m_piocpServer->Send(m_mainClientContext, (PBYTE)cmd, sizeof(HEADER));
}

void CClientDlg::SetIOCPServerAndContext(CIOCPServer* dwServer,DWORD dwMainClientContxt)
{
	m_piocpServer = dwServer;
	m_mainClientContext = (ClientContext*) dwMainClientContxt;
}
void CClientDlg::OnReceiveComplete(ClientContext* pContext)
{
	DATA* pData = (DATA*)pContext->m_DeCompressionBuffer.GetBuffer();
	switch(pData->header.flag)
	{
	case CLIENT_SHELL_START:
		m_cmdDlg->Start(m_piocpServer,pContext);
		break;
	case CLIENT_SHELL_DATA:
		m_cmdDlg->OnReceiveComplete(pContext);
		break;
	case CLIENT_FILE_START:
		m_fileDlg->Start(m_piocpServer,pContext);
		break;
	case CLIENT_FILE_DRIVE_LIST:
		m_fileDlg->OnReceiveDriveList(pContext);
		break;
	case CLIENT_FILE_LIST:
		m_fileDlg->OnReceiveFileList(pContext);
		break;
	case CLIENT_CREATE_FOLDER_FINISH:
		AfxMessageBox(_T("新建文件夹成功"));
		break;
	case CLIENT_CREATE_FOLDER_FAILED:
		AfxMessageBox(_T("新建文件夹失败"));
		break;
	case CLIENT_DLELE_FILE_FINISH:
		AfxMessageBox(_T("删除文件成功"));
		break;
	case CLIENT_DLELE_FILE_FAILED:
		AfxMessageBox(_T("删除文件失败"));
		break;
	case CLIENT_CREATE_FILE_FINISH:
		AfxMessageBox(_T("创建文件成功"));
		break;
	case CLIENT_CREATE_FILE_FAILED:
		AfxMessageBox(_T("创建文件成功"));
		break;
	case CLIENT_DELETE_FOLDER_FINISH:
		AfxMessageBox(_T("删除文件夹成功"));
		break;
	case CLIENT_DELETE_FOLDER_FAILED:
		AfxMessageBox(_T("删除文件夹失败"));
		break;
	case CLIENT_FILE_SIZE:
		m_fileDlg->OnReceiveFileSize(pContext);
		break;

	default:
		break;
	}
	return;
}
