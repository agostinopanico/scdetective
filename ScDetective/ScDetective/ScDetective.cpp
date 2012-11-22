
// ScDetect.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ScDetective.h"
#include "ScDetectiveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScDetectApp

BEGIN_MESSAGE_MAP(CScDetectiveApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CScDetectApp ����

CScDetectiveApp::CScDetectiveApp()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

//////////////////////////////////////////////////////////////////////////
//  ȫ�ֱ���
//////////////////////////////////////////////////////////////////////////
CScDetectiveApp theApp;                         // Ψһ��һ�� CScDetectApp ����

CDriver theDriver;                              // ȫ���豸����

#define LOAD_DRIVER

//////////////////////////////////////////////////////////////////////////

// CScDetectApp ��ʼ��

BOOL CScDetectiveApp::InitInstance()
{
    WORD wVersion;
    wVersion = GetCurrentOSVersion();
    switch (wVersion)
    {
    case VER_WINXP:
    case VER_WXPSP1:
    case VER_WXPSP2:
    case VER_WXPSP3:
    case VER_WINDOWS7:
        break;
    default:
        MessageBox(NULL, L"������֧������ʹ�õĲ���ϵͳ��", NULL, MB_OK);
        return FALSE;
    }

    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
    // �����ؼ��ࡣ
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
    
    //////////////////////////////////////////////////////////////////////////
    // ��������
#ifdef LOAD_DRIVER
    theDriver.LoadDriver(L"ScDetective.sys", L"ScDetective");
    ULONG ProcessId = GetCurrentProcessId();
    if (theDriver.m_hDriver && theDriver.m_hSCM) {
        theDriver.IoControl(IOCTL_PROTECT_MYSELF, &ProcessId, sizeof(ULONG), NULL, 0);
    }
    CloseMyHandles(ProcessId);
#endif
    
    CScDetectiveDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: �ڴ˷��ô����ʱ��
        //  ��ȷ�������رնԻ���Ĵ���
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: �ڴ˷��ô����ʱ��
        //  ��ȡ�������رնԻ���Ĵ���
    }

#ifdef LOAD_DRIVER
    // ж��ǰҪ����
    theDriver.IoControl(IOCTL_EXIT_PROCESS, NULL, 0, NULL, 0);
    // ����ж��
    theDriver.UnloadDriver();
#endif
    
    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
    //  ����������Ӧ�ó������Ϣ�á�
    return FALSE;
}
