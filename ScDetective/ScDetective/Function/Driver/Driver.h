///////////////////////////////////////////////////
// Driver.h�ļ�

#pragma once

#include <WinSvc.h>

class CDriver 
{
public:
    VOID LoadDriver(LPCTSTR pszDriverName, LPCTSTR pszLinkName);
    VOID UnloadDriver();

    // ���캯����pszDriverPathΪ��������Ŀ¼��pszLinkNameΪ������������
    // ����Ĺ��캯���У�����ͼ������򿪷���
    BOOL DoCDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName);
    // �����������������ֹͣ����
    VOID UnDoCDriver();

    // ����
    // �������Ƿ����
    BOOL IsValid() { return (m_hSCM != NULL && m_hService != NULL); }

    // ����
    // ��������Ҳ����˵������DriverEntry������������
    BOOL StartDriver();
    // �������񡣼����������DriverUnload���̽�������
    BOOL StopDriver();

    // ���豸����ȡ�õ���������һ�����
    BOOL OpenDevice();

    // ���豸���Ϳ��ƴ���
    DWORD IoControl(DWORD nCode, PVOID pInBuffer, 
                    DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount);
    // ʵ��
    WCHAR m_szLinkName[56];	// ������������

    BOOL  m_bStarted;	        // ָ�������Ƿ�����
    BOOL  m_bCreateService;	// ָ���Ƿ񴴽��˷���

    SC_HANDLE  m_hSCM;		// SCM���ݿ���
    SC_HANDLE  m_hService;	// ������

    HANDLE m_hDriver;	    // �豸���
};

extern CDriver theDriver;