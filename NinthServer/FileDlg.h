#pragma once
#include "IOCPServer.h"
#include "afxwin.h"
#include "afxcmn.h"

// CFileDlg 对话框

class CFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(CIOCPServer* pServer,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileDlg();

// 对话框数据
	enum { IDD = IDD_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CIOCPServer*		m_iocpServer;
	ClientContext*		m_Context;
	CImageList*			m_imageList;
	CEdit				m_currentPath;
	CTreeCtrl			m_dirTree;
	CListCtrl			m_fileList;
	HTREEITEM			m_selectDir;
	CEdit				m_input;
	CString				m_strCrtPath;
	CString				m_downloadFileName;
	CString				m_recvDownloadFileName;
	CString				m_sendFileName;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	void Start(CIOCPServer* pServer,void* pClient);
	void OnReceiveDriveList(ClientContext* pContext);
	void GetRemoteFileList();
	void OnReceiveFileList(ClientContext* pContext);
	void OnReceiveFileSize(ClientContext* pContext);
	afx_msg void OnTvnSelchangedDirTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeleteFile();
	afx_msg void OnCreatDic();
	
	afx_msg void OnCreatFile();
	afx_msg void OnTreeCreatedic();
	afx_msg void OnNMRClickDirTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreeDeletedic();
	afx_msg void OnDownloadFile();
};
