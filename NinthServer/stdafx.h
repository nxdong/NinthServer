
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


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
	IN_ADDR			IPAddress;		// 存储32位的IPv4的地址数据结构
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

//  协议标志
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


