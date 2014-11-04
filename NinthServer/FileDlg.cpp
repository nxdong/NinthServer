// FileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NinthServer.h"
#include "FileDlg.h"
#include "afxdialogex.h"

typedef struct
{
	CString	title;
	double		nWidth;
}COLUMNSTRUCT;
COLUMNSTRUCT m_List_Column_Data[] = 
{
	{_T("Name"),					0.25		},
	{_T("Size"),					0.25		},
	{_T("Attributes"),				0.25		},
	{_T("LastAccessTime"),			0.24		},

};
const int c_columnNum = 4;
// CFileDlg 对话框

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(CIOCPServer* pServer,CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileDlg::IDD, pParent)
{
	m_iocpServer	= pServer;
	HIMAGELIST hImageList;
	SHFILEINFO	sfi;
	SHGetFileInfo
		(
		_T("\\\\"),
		FILE_ATTRIBUTE_NORMAL, 
		&sfi,
		sizeof(SHFILEINFO), 
		SHGFI_ICON | SHGFI_USEFILEATTRIBUTES
		);
	hImageList = (HIMAGELIST)SHGetFileInfo
		(
		NULL,
		0,
		&sfi,
		sizeof(SHFILEINFO),
		SHGFI_SMALLICON | SHGFI_SYSICONINDEX
		);
	m_imageList = CImageList::FromHandle(hImageList);
}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_currentPath);
	DDX_Control(pDX, IDC_DIR_TREE, m_dirTree);
	DDX_Control(pDX, IDC_FILE_LIST, m_fileList);
	DDX_Control(pDX, IDC_EDIT2, m_input);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
	ON_WM_SIZE()

	ON_NOTIFY(TVN_SELCHANGED, IDC_DIR_TREE, &CFileDlg::OnTvnSelchangedDirTree)
	ON_NOTIFY(NM_RCLICK, IDC_FILE_LIST, &CFileDlg::OnNMRClickFileList)
	ON_COMMAND(ID_DELETE_FILE, &CFileDlg::OnDeleteFile)

	ON_COMMAND(ID_CREAT_FILE, &CFileDlg::OnCreatFile)
	ON_COMMAND(ID_TREE_CREATEDIC, &CFileDlg::OnTreeCreatedic)
	ON_NOTIFY(NM_RCLICK, IDC_DIR_TREE, &CFileDlg::OnNMRClickDirTree)
	ON_COMMAND(ID_TREE_DELETEDIC, &CFileDlg::OnTreeDeletedic)
	ON_COMMAND(ID_DOWNLOAD_FILE, &CFileDlg::OnDownloadFile)
END_MESSAGE_MAP()


// CFileDlg 消息处理程序
void CFileDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (NULL == m_fileList)
	{
		return;
	}
	CRect rect;
	this->GetClientRect(&rect);
	CRect	treeRect(rect);
	treeRect.top	+= 25;
	treeRect.right	= 155;
	m_dirTree.MoveWindow(treeRect);
	CRect	listRect(rect);
	listRect.left += 155;
	listRect.top  += 25;
	m_fileList.MoveWindow(listRect);
	int width = rect.Width()-155;
	for (int i=0;i<c_columnNum;i++)
	{
		m_fileList.SetColumnWidth(i,(int)(width*m_List_Column_Data[i].nWidth));
	}

	// TODO: 在此处添加消息处理程序代码
}
void CFileDlg::Start(CIOCPServer* dwServer,void* pClient)
{
	m_iocpServer    = dwServer;
	m_Context		= (ClientContext*)pClient;
	// 通知远程控制端对话框已经打开
	HEADER	header;
	header.flag	= SERVER_NEXT;
	m_iocpServer->Send(m_Context, (LPBYTE)&header, sizeof(HEADER));
}

BOOL CFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_dirTree.SetImageList(m_imageList, TVSIL_NORMAL);

	DWORD dwStyle = m_fileList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_fileList.SetExtendedStyle(dwStyle); //设置扩展风格

	CRect rect;
	this->GetClientRect(&rect);
	int width = rect.right - rect.left - 155;

	for (int i=0;i<c_columnNum;i++)
	{
		m_fileList.InsertColumn(i,m_List_Column_Data[i].title,LVCFMT_LEFT,(int)(width*m_List_Column_Data[i].nWidth));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFileDlg::OnReceiveDriveList(ClientContext* pContext)
{
	HEADER *pHeader = (HEADER*)pContext->m_DeCompressionBuffer.GetBuffer();
	PBYTE driveData = new BYTE[pHeader->datalength];
	memcpy(driveData,pContext->m_DeCompressionBuffer.GetBuffer(sizeof(HEADER)),pHeader->datalength);
	wchar_t* pDrive;
	pDrive = (wchar_t*)driveData;
	m_dirTree.DeleteAllItems();
	HTREEITEM hItem;
	//TVINSERTSTRUCT tvInsert;
	do 
	{
// 		tvInsert.hParent = NULL;
// 		tvInsert.hInsertAfter = NULL;
// 		tvInsert.item.mask = TVIF_TEXT|TVIF_CHILDREN;
// 		tvInsert.item.pszText = pDrive;
// 
// 		hItem = m_dirTree.InsertItem(&tvInsert);
		hItem = m_dirTree.InsertItem(pDrive,0,0);
		m_dirTree.SetItemState(hItem,TVS_HASBUTTONS ,TVS_HASBUTTONS );
		pDrive += (lstrlen(pDrive)+1);
	} while (*pDrive !='\x00');
}

void CFileDlg::OnTvnSelchangedDirTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_fileList.DeleteAllItems();
	CString strText; // 树节点的标签文本字符串   
	HTREEITEM hItem = m_dirTree.GetSelectedItem(); 
	m_selectDir	 = hItem;
   // 获取选中节点的标签文本字符串   
    strText = m_dirTree.GetItemText(hItem); 
	m_strCrtPath = strText;
	hItem =  m_dirTree.GetParentItem(hItem);
	while(hItem)
	{
		strText = m_dirTree.GetItemText(hItem); 
		m_strCrtPath = strText + _T("\\") + m_strCrtPath;
		hItem =  m_dirTree.GetParentItem(hItem);
	}
	m_currentPath.SetWindowText(m_strCrtPath);

	GetRemoteFileList();

}

void CFileDlg::GetRemoteFileList()
{
	CString strCrtPath;
	m_currentPath.GetWindowText(strCrtPath);
	HEADER pHeader;
	pHeader.flag = SERVER_FILE_LIST;
	pHeader.datalength = 2 * strCrtPath.GetLength();
	BYTE *pData = new BYTE[sizeof(HEADER)+pHeader.datalength];
	memcpy(pData,&pHeader,sizeof(HEADER));
	memcpy(pData+sizeof(HEADER),strCrtPath.GetBuffer(),pHeader.datalength);
	m_iocpServer->Send(m_Context,pData,sizeof(HEADER)+pHeader.datalength);
	
	m_dirTree.EnableWindow(FALSE);
	delete pData;
}

void CFileDlg::OnReceiveFileList(ClientContext* pContext)
{
	FILE_LIST_DATA *fileListData = (FILE_LIST_DATA *)pContext->m_DeCompressionBuffer.GetBuffer();

	m_dirTree.EnableWindow(TRUE);
	//m_fileList.DeleteAllItems();
	//m_dirTree.DeleteAllItems();
	
	int nCnt,nCrtCnt ; 
	for (int i = 0;i<10;i++)
	{
		if (fileListData->findData[i].dwFileAttributes == 0 )
			break;
		// 将文件夹加入目录树
		if (fileListData->findData[i].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
 			if (!lstrcmp(fileListData->findData[i].cFileName,_T(".")) || !lstrcmp(fileListData->findData[i].cFileName,_T("..")))
 			{
 				continue;
 			}
			m_dirTree.InsertItem(fileListData->findData[i].cFileName,0,0,m_selectDir);
			continue;
		}
		//将其他文件加入文件视图
		nCnt = m_fileList.GetItemCount();
		nCrtCnt = m_fileList.InsertItem(nCnt,fileListData->findData[i].cFileName);
		CString temp;
		temp.Format(_T("%d%d"),
			fileListData->findData[i].nFileSizeHigh,
			fileListData->findData[i].nFileSizeLow);
		m_fileList.SetItemText(nCrtCnt,1,temp);
		temp.Format(_T("%d"),
			fileListData->findData[i].dwFileAttributes);
		m_fileList.SetItemText(nCrtCnt,2,temp);
		SYSTEMTIME st;
		wchar_t szLocalDate[255],szLocalTime[255];
		FileTimeToLocalFileTime( &fileListData->findData[i].ftLastAccessTime, &fileListData->findData[i].ftLastAccessTime);
		FileTimeToSystemTime( &fileListData->findData[i].ftLastAccessTime, &st );
		GetDateFormat( LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, NULL,
			szLocalDate, 255 );
		GetTimeFormat( LOCALE_USER_DEFAULT, 0, &st, NULL, szLocalTime, 255 );
		temp.Format(_T("%s-%s"),szLocalDate,szLocalTime);
		m_fileList.SetItemText(nCrtCnt,3,temp);
	}

}

void CFileDlg::OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (m_fileList.GetSelectedCount() <= 0)
	{
		return;
	}
	CMenu menu, *pPopup;
	menu.LoadMenu(IDR_FILE_LIST_MENU);
	pPopup = menu.GetSubMenu(0);
	CPoint myPoint;
	ClientToScreen(&myPoint);
	GetCursorPos(&myPoint); //鼠标位置
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this);
	*pResult = 0;
}



void CFileDlg::OnDeleteFile()
{
	// TODO: 在此添加命令处理程序代码
	int cnt = m_fileList.GetSelectedCount();
	POSITION pos = m_fileList.GetFirstSelectedItemPosition(); 
	HEADER pHeader;
	pHeader.flag = SERVER_DELETE_FILE;
	BYTE pData[600];
	

	while(pos) 
	{
		memset(pData,0,sizeof(pData));
		int nItem = m_fileList.GetNextSelectedItem(pos);
		CString	file = m_strCrtPath + m_fileList.GetItemText(nItem, 0);
		pHeader.datalength = file.GetLength() * sizeof(TCHAR);/*file.GetLength() * 2;*/
		memcpy(pData,&pHeader,sizeof(HEADER));
		memcpy(pData+sizeof(HEADER),file.GetBuffer(),pHeader.datalength);
		m_iocpServer->Send(m_Context,(LPBYTE)pData,pHeader.datalength+sizeof(HEADER));
	} 
	
}




void CFileDlg::OnCreatFile()
{
	// TODO: 在此添加命令处理程序代码
	CString strInput;
	m_input.GetWindowTextW(strInput);
	strInput = m_strCrtPath + strInput;
	HEADER pHeader;
	pHeader.flag = SERVER_CREATE_FILE;
	pHeader.datalength = strInput.GetLength()*sizeof(TCHAR);
	PBYTE pData = new BYTE[pHeader.datalength+sizeof(HEADER)];
	memset(pData,0,sizeof(HEADER)+pHeader.datalength);
	memcpy(pData,&pHeader,sizeof(HEADER));

	memcpy(pData+sizeof(HEADER),strInput.GetBuffer(),pHeader.datalength);
	m_iocpServer->Send(m_Context,pData,sizeof(HEADER)+pHeader.datalength);


}


void CFileDlg::OnTreeCreatedic()
{
	// TODO: 在此添加命令处理程序代码
	CString strInput;
	m_input.GetWindowTextW(strInput);
	strInput = m_strCrtPath + strInput;
	HEADER pHeader;
	pHeader.flag = SERVER_CREATE_FOLDER;
	pHeader.datalength = strInput.GetLength()*sizeof(TCHAR);
	PBYTE pData = new BYTE[pHeader.datalength+sizeof(HEADER)];
	memset(pData,0,sizeof(HEADER)+pHeader.datalength);
	memcpy(pData,&pHeader,sizeof(HEADER));

	memcpy(pData+sizeof(HEADER),strInput.GetBuffer(),pHeader.datalength);
	m_iocpServer->Send(m_Context,pData,sizeof(HEADER)+pHeader.datalength);

}


void CFileDlg::OnNMRClickDirTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
	if (m_dirTree.GetSelectedCount() <= 0)
	{
		return;
	}
	CMenu menu, *pPopup;
	menu.LoadMenu(IDR_DIR_TREE_VIEW);
	pPopup = menu.GetSubMenu(0);
	CPoint myPoint;
	ClientToScreen(&myPoint);
	GetCursorPos(&myPoint); //鼠标位置
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this);
	*pResult = 0;
}


void CFileDlg::OnTreeDeletedic()
{
	// TODO: 在此添加命令处理程序代码
	CString strText; // 树节点的标签文本字符串   
	HTREEITEM hItem = m_dirTree.GetSelectedItem(); 
	// 获取选中节点的标签文本字符串   
	strText = m_dirTree.GetItemText(hItem); 
	CString dicpath;
	dicpath = strText;
	hItem =  m_dirTree.GetParentItem(hItem);
	while(hItem)
	{
		strText = m_dirTree.GetItemText(hItem); 
		dicpath = strText + _T("\\") +dicpath;
		hItem =  m_dirTree.GetParentItem(hItem);
	}
	

	HEADER pHeader;
	pHeader.flag = SERVER_DELETE_FOLDER;
	pHeader.datalength = dicpath.GetLength()*sizeof(TCHAR);
	PBYTE pData = new BYTE[pHeader.datalength+sizeof(HEADER)];
	memset(pData,0,sizeof(HEADER)+pHeader.datalength);
	memcpy(pData,&pHeader,sizeof(HEADER));

	memcpy(pData+sizeof(HEADER),dicpath.GetBuffer(),pHeader.datalength);
	m_iocpServer->Send(m_Context,pData,sizeof(HEADER)+pHeader.datalength);

}


void CFileDlg::OnDownloadFile()
{
	// TODO: 在此添加命令处理程序代码
	CString strText;
	POSITION pos = m_fileList.GetFirstSelectedItemPosition(); 
	int nItem = m_fileList.GetNextSelectedItem(pos);
	strText = m_fileList.GetItemText(nItem, 0);
	m_downloadFileName = m_strCrtPath + strText;

	HEADER header;
	header.flag = SERVER_DOWNLOAD_FILE;
	header.datalength = m_downloadFileName.GetLength()*sizeof(TCHAR);

	BYTE *pData = new BYTE[sizeof(HEADER)+header.datalength];
	memset(pData,0,sizeof(HEADER)+header.datalength);
	memcpy(pData,&header,sizeof(HEADER));
	memcpy(pData+sizeof(HEADER),m_downloadFileName.GetBuffer(),header.datalength);

	m_iocpServer->Send(m_Context,pData,sizeof(HEADER)+header.datalength);

	delete pData;

}
void CFileDlg::OnReceiveFileSize(ClientContext* pContext)
{
	HEADER *pHeader = (HEADER*)pContext->m_DeCompressionBuffer.GetBuffer();
	FILESIZE *pFileSize = (FILESIZE*)pContext->m_DeCompressionBuffer.GetBuffer(sizeof(HEADER));
	wchar_t* pFileName = (wchar_t*)pContext->m_DeCompressionBuffer.GetBuffer(sizeof(HEADER)+sizeof(FILESIZE));

}
