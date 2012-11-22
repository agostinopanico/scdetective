
// ScDetectDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "Page4.h"
#include "Page5.h"
#include "PageFile.h"
#include "afxwin.h"

#define SC_PAGE_NUM     6

// CScDetectDlg �Ի���
class CScDetectiveDlg : public CDialog
{
    // ����
public:
    CScDetectiveDlg(CWnd* pParent = NULL);	// ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_SCDETECTIVE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


    // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    CTabCtrl theTable;
    CStatic m_Status;
    CPage1 m_Page1;
    CPage2 m_Page2;
    CPage3 m_Page3;
    CPage4 m_Page4;
    CPage5 m_Page5;
    CPageFile m_Page6;

    CDialog* thePageArray[SC_PAGE_NUM];

public:
    VOID ShowPageX(int nIndex);
public:
    afx_msg void OnTcnSelectChangeTabPage(NMHDR *pNMHDR, LRESULT *pResult);

};
