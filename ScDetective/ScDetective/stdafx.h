
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
#include "Resource.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxcmn.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h> 
#include <commctrl.h>  
#include <stdio.h>
#include <string.h>

// windows Header Files 
#include <windows.h>
#include <Winsvc.h>
#include <Winuser.h>
#include <Winbase.h>
#include <WinIoCtl.h>

#pragma comment(lib, "version.lib")

//////////////////////////////////////////////////////////////////////////
// Common
//////////////////////////////////////////////////////////////////////////
#include "../../Common/ring3common.h"
#include "../../Common/DataStruct.h"
#include "../../Common/IoControlCmd.h"
#include "../../Common/MajorFunctionName.h"
#include "../../Common/VistaShadowSSDT.h"
#include "../../Common/W2K3ShadowSSDT.h"
#include "../../Common/W2KShadowSSDT.h"
#include "../../Common/XPShadowSSDT.h"
#include "../../Common/Win7ShadowSSDT.h"

//////////////////////////////////////////////////////////////////////////
// Function
//////////////////////////////////////////////////////////////////////////
// ssdt
#include "Function/ssdt/ssdt.h"
// Driver
#include "Function/Driver/Driver.h"
// Module
#include "Function/module/Module.h"
// Process
#include "Function/module/Process.h"
// OS
#include "Function/os/OS.h"
// PE
#include "Function/pe/PE.h"
// File
#include "Function/File/File.h"

//////////////////////////////////////////////////////////////////////////
// Dialog
//////////////////////////////////////////////////////////////////////////
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "Page4.h"
#include "Page5.h"



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
// #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


