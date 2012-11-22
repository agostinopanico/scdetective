// Page1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Page1.h"
#include "ScDetective.h"
#include <WinIoCtl.h>

extern CDriver theDriver;
extern CStatic* theStatus;

// CPage1 �Ի���

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
: CDialog(CPage1::IDD, pParent)
{
    pSsdtName = NULL;
    pSsdtNativeAddress = NULL;
    NumOfFunName = 0;
    NumOfNativeAddress = 0;
    memset(StatusBuffer, 0, 128 * 2);
    pSsdtName = GetSsdtNativeFunNames(&NumOfFunName);
    pSsdtNativeAddress = GetSsdtNativeFunAddresses(&NumOfNativeAddress);
}

CPage1::~CPage1()
{
    if (pSsdtNativeAddress)  GlobalFree(pSsdtNativeAddress);
    if (pSsdtName)  GlobalFree(pSsdtName);
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_ListSsdt);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
    ON_COMMAND(ID_MENU_32771, &CPage1::OnMenuRefresh)
    ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CPage1::OnNMRClickListSsdt)
    ON_COMMAND(ID_MENU_32772, &CPage1::OnMenuSsdtUnhook)
    ON_COMMAND(ID_MENU_32773, &CPage1::OnMenuSsdtUnhookAll)
    ON_COMMAND(ID_MENU_32775, &CPage1::OnMenuShowAttribute)
    ON_COMMAND(ID_MENU_32774, &CPage1::OnMenuLocateToFile)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CPage1::OnNMCustomdrawSsdtListColor)
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CPage1 ��Ϣ�������

BOOL CPage1::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_ListSsdt.SetExtendedStyle(m_ListSsdt.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_ListSsdt.InsertColumn( 0, L"�����",     LVCFMT_CENTER, 60, -1);
    m_ListSsdt.InsertColumn( 1, L"��������", LVCFMT_LEFT, 170, -1);
    m_ListSsdt.InsertColumn( 2, L"��ǰ��ַ",   LVCFMT_CENTER, 100, -1);
    m_ListSsdt.InsertColumn( 3, L"Hook",      LVCFMT_CENTER, 80, -1);
    m_ListSsdt.InsertColumn( 4, L"ԭʼ��ַ",   LVCFMT_CENTER, 100, -1);  
    m_ListSsdt.InsertColumn( 5, L"ģ����",     LVCFMT_LEFT, 250, -1);
    
    OnMenuRefresh();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CPage1::OnMenuRefresh()
{
    m_ListSsdt.DeleteAllItems();
    if (theDriver.m_hDriver == NULL || 
        theDriver.m_hDriver == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD BytesReturned;
    ULONG NeedLen;
    PSSDT_ADDRESS pCurrentSsdtAddr = NULL;
    SSDT_INFO SsdtInfo = { 0 };
    UINT nChanged = 0;
    BOOL ret = FALSE;

    BytesReturned = theDriver.IoControl(IOCTL_GET_SSDT, 
                                        NULL, 0, 
                                        &NeedLen, 
                                        sizeof(ULONG)
                                        );
    pCurrentSsdtAddr = (PSSDT_ADDRESS)GlobalAlloc(GPTR, NeedLen);

    if (pCurrentSsdtAddr != NULL)
    {
        BytesReturned = theDriver.IoControl(IOCTL_GET_SSDT, 
                                            NULL, 0, 
                                            pCurrentSsdtAddr, 
                                            NeedLen);
    }
    else return ;

    for (ULONG i = 0; i < NumOfNativeAddress; i++)
    {
        SsdtInfo.NativeAddress  = pSsdtNativeAddress[i];
        SsdtInfo.CurrentAddress = pCurrentSsdtAddr[i].FunAddress;
        SsdtCurrentAddress[i] = pCurrentSsdtAddr[i].FunAddress;
        SsdtInfo.Index = i;
        SsdtInfo.Hideflag = FALSE;

        if (SsdtInfo.CurrentAddress != SsdtInfo.NativeAddress) {
            SsdtInfo.Hideflag = TRUE;
        }
        if (pSsdtName != NULL) {
            lstrcpyA(SsdtInfo.FunName, pSsdtName[i].FunName);
        } else {
            lstrcpyA(SsdtInfo.FunName, "δ֪����");
        }

        char szPath[MAX_PATH] = { 0 };

        ret = GetKernelModuleNameByAddress(szPath, MAX_PATH, SsdtInfo.CurrentAddress, TRUE);
        if (ret == TRUE)
        {
            // lstrcpyA(SsdtInfo.ImagePath, szPath);
            ModifyFileImagePath(szPath, SsdtInfo.ImagePath, 260);
        }

        WCHAR szIndex[16];
        WCHAR szServiceName[64];
        WCHAR szCurrentAddr[16];
        WCHAR szNativeAddr[16];
        WCHAR szImagePath[MAX_PATH];

        wsprintf(szIndex, L"%03d", i);
        wsprintf(szServiceName, L"%S", SsdtInfo.FunName);
        wsprintf(szCurrentAddr, L"0x%08X", SsdtInfo.CurrentAddress);
        wsprintf(szNativeAddr, L"0x%08X", SsdtInfo.NativeAddress);
        MultiByteToWideChar(CP_ACP, 0, SsdtInfo.ImagePath, MAX_PATH, szImagePath, MAX_PATH);

        int nItemNum = m_ListSsdt.GetItemCount();
        m_ListSsdt.InsertItem(nItemNum, szIndex);
        m_ListSsdt.SetItemText(nItemNum, 1, szServiceName);
        m_ListSsdt.SetItemText(nItemNum, 2, szCurrentAddr);
        m_ListSsdt.SetItemText(nItemNum, 4, szNativeAddr);
        m_ListSsdt.SetItemText(nItemNum, 5, szImagePath);

        if (SsdtInfo.Hideflag) {
            ++ nChanged;
            m_ListSsdt.SetItemText(nItemNum, 3, L"ssdt hook");
        } else {
            m_ListSsdt.SetItemText(nItemNum, 3, L"-");
        }
    }
    wsprintf(StatusBuffer, L"ssdt ���� :: %d - ���ҹ����� :: %d", NumOfNativeAddress, nChanged);
    theStatus->SetWindowText(StatusBuffer);

    GlobalFree(pCurrentSsdtAddr);
}

void CPage1::OnNMRClickListSsdt(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
    {
        CPoint pt;
        GetCursorPos(&pt);
        CMenu menu;
        menu.LoadMenu(IDR_MENU1_SSDT);
        CMenu* pMenu = menu.GetSubMenu(0);
        pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
    }
    *pResult = 0;
}

void CPage1::OnMenuSsdtUnhook()
{
    SSDT_ADDRESS SsdtInfo = {0};
    WCHAR szBuffer[16] = {0};
    WCHAR szCurrentAddr[16] = {0};
    WCHAR szOrigAddr[16] = {0};
    WCHAR szIndex[16] = {0};
    ULONG CurrentAddress;
    ULONG OrigAddress;
    POSITION pos = m_ListSsdt.GetFirstSelectedItemPosition();
    
    if (pos)
    {
        int nItem = m_ListSsdt.GetNextSelectedItem(pos);
        m_ListSsdt.GetItemText(nItem, 2, szCurrentAddr, 16);
        swscanf_s(szCurrentAddr + 2, L"%X", &CurrentAddress);
        m_ListSsdt.GetItemText(nItem, 3, szOrigAddr, 16);
        swscanf_s(szOrigAddr + 2, L"%X", &OrigAddress);
        
        if (CurrentAddress == OrigAddress) goto theEnd;

        m_ListSsdt.GetItemText(nItem, 0, szIndex, 16);
        SsdtInfo.nIndex = _wtol(szIndex);
        SsdtInfo.FunAddress = OrigAddress;

        ULONG BytesReturned = 
            theDriver.IoControl(IOCTL_UNHOOK_SSDT, &SsdtInfo, sizeof(SSDT_ADDRESS), NULL, 0);
    }
theEnd:
    OnMenuRefresh();
}

void CPage1::OnMenuSsdtUnhookAll()
{
    // TODO: �ڴ���������������
}

void CPage1::OnMenuShowAttribute()
{
    // TODO: �ڴ���������������
}

void CPage1::OnMenuLocateToFile()
{
    // TODO: �ڴ���������������
}

void CPage1::OnNMCustomdrawSsdtListColor(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    // TODO: �ڴ���ӿؼ�֪ͨ����������
    *pResult = CDRF_DODEFAULT;;

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
        *pResult = CDRF_NOTIFYITEMDRAW;
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
    {
        COLORREF clrNewTextColor, clrNewBkColor;
        int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec);

        //�����ǰSSDT��ַ��ԭʼ��ַ��һ��,����ʾ����Ϊ��ɫ,����Ϊ��ɫ
        if(pSsdtNativeAddress[nItem] != SsdtCurrentAddress[nItem])    //����Ϊ��ɫ
            clrNewTextColor = RGB(255, 0, 0);
        else		//����Ϊ��ɫ
            clrNewTextColor = RGB(0, 0, 0);

        //���ñ���ɫ
        if( nItem % 2 == 0)
            clrNewBkColor = RGB(240, 240, 240);	    //ż���б���ɫΪ��ɫ
        else
            clrNewBkColor = RGB(255, 255, 255);	    //�����б���ɫΪ��ɫ

        pLVCD->clrText = clrNewTextColor;
        pLVCD->clrTextBk = clrNewBkColor;

        *pResult = CDRF_DODEFAULT;
    }
}

void CPage1::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);

    // TODO: �ڴ˴������Ϣ����������
    theStatus->SetWindowText(StatusBuffer);
}
