
// #include "../../stdafx.h"
// ʹ��Ԥ����ͷ
#include "stdafx.h"

BOOL CDriver::DoCDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName)
{
    wcscpy_s(m_szLinkName, 55, pszLinkName);
    m_bStarted = FALSE;
    m_bCreateService = FALSE;
    m_hSCM = m_hService = NULL;
    m_hDriver = INVALID_HANDLE_VALUE;

    // ��SCM������
    m_hSCM = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if(m_hSCM == NULL)
    {
        MessageBox(0, L"�򿪷�����ƹ�����ʧ��\n", 
            L"��������Ϊ����ӵ��AdministratorȨ��\n", 0);
        return FALSE;
    }

    // ������򿪷���
    m_hService = ::CreateServiceW(m_hSCM, m_szLinkName, m_szLinkName, SERVICE_ALL_ACCESS, 
        SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, 
        pszDriverPath, NULL, 0, NULL, NULL, NULL);
    if(m_hService == NULL)
    {
        // ��������ʧ�ܣ���������Ϊ�����Ѿ����ڣ����Ի�Ҫ��ͼ����
        int nError = ::GetLastError();
        if(nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
        {
            m_hService = ::OpenServiceW(m_hSCM, m_szLinkName, SERVICE_ALL_ACCESS);
        }
        if (m_hService == NULL)
        {
            MessageBox(0, L"CreateServiceW\n", 
                L"��������Ϊ����ӵ��AdministratorȨ��\n", 0);
        }
        else
        {
            m_bCreateService = TRUE;
        }
    }
    else
    {
        m_bCreateService = TRUE;
    }
    return m_bCreateService;
}

VOID CDriver::UnDoCDriver()
{
    // �ر��豸���
    if(m_hDriver != INVALID_HANDLE_VALUE)
        ::CloseHandle(m_hDriver);
    // ��������˷��񣬾ͽ�֮ɾ��
    if(m_bCreateService)
    {
        StopDriver();
        ::DeleteService(m_hService);	
    }
    // �رվ��
    if(m_hService != NULL)
        ::CloseServiceHandle(m_hService);
    if(m_hSCM != NULL)
        ::CloseServiceHandle(m_hSCM);
}

BOOL CDriver::StartDriver()
{
    if(m_bStarted)
        return TRUE;
    if(m_hService == NULL)
        return FALSE;
    // ��������
    if(!::StartServiceW(m_hService, 0, NULL))
    {
        int nError = ::GetLastError();
        if(nError == ERROR_SERVICE_ALREADY_RUNNING)
            m_bStarted = TRUE;
        else
            ::DeleteService(m_hService);
    }
    else
    {
        // �����ɹ��󣬵ȴ������������״̬
        int nTry = 0;
        SERVICE_STATUS ss;
        ::QueryServiceStatus(m_hService, &ss);
        while(ss.dwCurrentState == SERVICE_START_PENDING && nTry++ < 80)
        {
            ::Sleep(50);
            ::QueryServiceStatus(m_hService, &ss);
        }
        if(ss.dwCurrentState == SERVICE_RUNNING)
            m_bStarted = TRUE;
    }
    return m_bStarted;
}

BOOL CDriver::StopDriver()
{
    if(!m_bStarted)
        return TRUE;
    if(m_hService == NULL)
        return FALSE;
    // ֹͣ����
    SERVICE_STATUS ss;
    if(!::ControlService(m_hService, SERVICE_CONTROL_STOP, &ss))
    {
        if(::GetLastError() == ERROR_SERVICE_NOT_ACTIVE)
            m_bStarted = FALSE;
    }
    else
    {
        // �ȴ�������ȫֹͣ����
        int nTry = 0;
        while(ss.dwCurrentState == SERVICE_STOP_PENDING && nTry++ < 80)
        {
            ::Sleep(50);
            ::QueryServiceStatus(m_hService, &ss);
        }
        if(ss.dwCurrentState == SERVICE_STOPPED)
            m_bStarted = FALSE;
    }
    return !m_bStarted;
}

BOOL CDriver::OpenDevice()
{
    if(m_hDriver != INVALID_HANDLE_VALUE)
        return TRUE;

    // "\\.\"��Win32�ж��屾�ؼ�����ķ�����
    WCHAR sz[256] = { 0 };
    wsprintf(sz, L"\\\\.\\%s", m_szLinkName);
    // �����������������豸
    m_hDriver = ::CreateFile(sz,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    return (m_hDriver != INVALID_HANDLE_VALUE);
}

DWORD CDriver::IoControl(DWORD nCode, PVOID pInBuffer, 
                         DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount)
{
    if(m_hDriver == INVALID_HANDLE_VALUE)
        return -1;
    // �����������Ϳ��ƴ���
    DWORD nBytesReturn;
    BOOL bRet = ::DeviceIoControl(m_hDriver, nCode, 
        pInBuffer, nInCount, pOutBuffer, nOutCount, &nBytesReturn, NULL);
    if(bRet)
        return nBytesReturn;
    else
        return -1;
}

VOID CDriver::LoadDriver(LPCTSTR pszDriverName, LPCTSTR pszLinkName)
{
    WCHAR szDriverImagePath[256] = { 0 };

    GetModuleFileName(NULL, szDriverImagePath, MAX_PATH);

    for (int i = wcslen(szDriverImagePath); i > 0; i --)
    {
        if (szDriverImagePath[i] == L'\\')
        {
            szDriverImagePath[i+1] = L'\0';
            break;
        }
    }

    wcscat_s(szDriverImagePath, 256, pszDriverName);

    if (!DoCDriver(szDriverImagePath, pszLinkName))
    {
        MessageBox(NULL, L"Install ScDriver failed...", NULL, MB_OK);
        return;
    }
    if (!StartDriver())
    {
        MessageBox(NULL, L"Start ScDriver failed...", NULL, MB_OK);
        UnDoCDriver();
        return;
    }
    if (!IsValid())
    {
        MessageBox(NULL, L"ScDriver is not valid...", NULL, MB_OK);
        UnDoCDriver();
        return;
    }
    if (!OpenDevice())
    {
        MessageBox(NULL, L"Open ScDriver failed...", NULL, MB_OK);
        UnDoCDriver();
        return;
    }
}

VOID CDriver::UnloadDriver()
{
    UnDoCDriver();
}
