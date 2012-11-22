
// ScDetectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScDetective.h"
#include "ScDetectiveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CDriver theDriver;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    // ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CScDetectDlg �Ի���

CStatic* theStatus;


CScDetectiveDlg::CScDetectiveDlg(CWnd* pParent /*=NULL*/)
: CDialog(CScDetectiveDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScDetectiveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_PAGE, theTable);
    DDX_Control(pDX, IDC_STATUS_TEXT, m_Status);
}

BEGIN_MESSAGE_MAP(CScDetectiveDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAGE, &CScDetectiveDlg::OnTcnSelectChangeTabPage)
END_MESSAGE_MAP()


// CScDetectDlg ��Ϣ�������

BOOL CScDetectiveDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    ShowWindow(SW_NORMAL);

    theStatus = &m_Status;

    thePageArray[0] = &m_Page1;
    thePageArray[1] = &m_Page2;
    thePageArray[2] = &m_Page3;
    thePageArray[3] = &m_Page4;
    thePageArray[4] = &m_Page5;
    thePageArray[5] = &m_Page6;

    theTable.MoveWindow(CRect(0,0,830,20), FALSE);

    theTable.InsertItem(0, L"ssdt");
    thePageArray[0]->Create(IDD_SSDT, this);

    theTable.InsertItem(1, L"ssdt shadow");
    thePageArray[1]->Create(IDD_SSDTSHADOW, this);

    theTable.InsertItem(2, L"Ӧ�ò㹳��");
    thePageArray[2]->Create(IDD_PAGE_R3HOOK, this);

    theTable.InsertItem(3, L"���̹���");
    thePageArray[3]->Create(IDD_PAGE_PROCESS, this);

    theTable.InsertItem(4, L"������Ϣ");
    thePageArray[4]->Create(IDD_PAGE_DRIVEROBJECT, this);

    theTable.InsertItem(5, L"�ļ�����");
    thePageArray[5]->Create(IDD_PAGE_FILE, this);

    theTable.SetCurSel(0);

    RECT rect;
    this->GetClientRect(&rect);
    rect.left   +=  5;
    rect.top    +=  22;
    rect.bottom +=  20;

    for (int i = 0; i < SC_PAGE_NUM; i++)
    {
        thePageArray[i]->MoveWindow(&rect);
    }

    thePageArray[0]->ShowWindow(SW_SHOW);

    this->GetClientRect(&rect);
    rect.left   +=  20;
    rect.bottom +=  5;
    rect.top    =   rect.bottom - 30;

    m_Status.MoveWindow(&rect);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CScDetectiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScDetectiveDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CScDetectiveDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CScDetectiveDlg::OnTcnSelectChangeTabPage(NMHDR *pNMHDR, LRESULT *pResult)
{
    int nCurSelect = theTable.GetCurSel();
    ShowPageX(nCurSelect);
    *pResult = 0;
}

VOID CScDetectiveDlg::ShowPageX(int nIndex)
{
    for (int i = 0; i < SC_PAGE_NUM; i++)
    {
        thePageArray[i]->ShowWindow(SW_HIDE);
    }
    thePageArray[nIndex]->BringWindowToTop();
    thePageArray[nIndex]->ShowWindow(SW_SHOWNORMAL);
    thePageArray[nIndex]->UpdateWindow();
}

