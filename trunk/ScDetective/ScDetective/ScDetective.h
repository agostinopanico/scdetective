
// ScDetect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CScDetectApp:
// �йش����ʵ�֣������ ScDetect.cpp
//

class CScDetectiveApp : public CWinApp
{
public:
    CScDetectiveApp();

    // ��д
public:
    virtual BOOL InitInstance();

    // ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CScDetectiveApp theApp;