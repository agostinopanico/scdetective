

#ifndef _PAGE5_H_
#define _PAGE5_H_

// CPage5 �Ի���

class CPage5 : public CDialog
{
	DECLARE_DYNAMIC(CPage5)

public:
	CPage5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage5();

// �Ի�������
	enum { IDD = IDD_PAGE_DRIVEROBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListDriver;
    WCHAR  StatusBuffer[128];
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnNMCustomdrawListDrivers(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMenuRefreshDrivers();
    afx_msg void OnNMRClickListDrivers(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif



