
#ifndef _PAGE3_H_
#define _PAGE3_H_

#define PAGE_R3_NUMBER      2

//////////////////////////////////////////////////////////////////////////
// CMsgHook �Ի���

class CMsgHook : public CDialog
{
    DECLARE_DYNAMIC(CMsgHook)

public:
    CMsgHook(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CMsgHook();

    // �Ի�������
    enum { IDD = IDD_MSGHOOK };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
private:
    CListCtrl m_ListMsgHook;
    ULONG m_pSharedInfo;

    ULONG LocateSharedInfo();
    BOOL DumpKernelMemory(ULONG pvAddress, ULONG cbDump, PVOID Buffer);

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnMenuRefreshMsgHook();
    afx_msg void OnMenuUnhook();
    afx_msg void OnMenuLocate2File();
    afx_msg void OnNMRClickListMsghook(NMHDR *pNMHDR, LRESULT *pResult);
};

//////////////////////////////////////////////////////////////////////////
// CSystemRoutine �Ի���

class CSystemRoutine : public CDialog
{
    DECLARE_DYNAMIC(CSystemRoutine)

public:
    CSystemRoutine(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CSystemRoutine();

    // �Ի�������
    enum { IDD = IDD_SYSROUTINE };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
// CPage3 �Ի���

class CPage3 : public CDialog
{
    DECLARE_DYNAMIC(CPage3)

public:
    CPage3(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CPage3();

    // �Ի�������
    enum { IDD = IDD_PAGE_R3HOOK };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()

private:
    CTabCtrl tableHook;
    CMsgHook m_Page1;
    CSystemRoutine m_Page2;
    CDialog* PageArrayR3Hook[PAGE_R3_NUMBER];

public:
    VOID ShowPageX(int nIndex);
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnTcnSelchangeTabR3Hook(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

typedef enum  _HANDLE_TYPE {
    TYPE_FREE = 0  ,                  //must be zero!
    TYPE_WINDOW = 1 ,                 //in order of use for C code lookups
    TYPE_MENU = 2,
    TYPE_CURSOR = 3,
    TYPE_SETWINDOWPOS = 4,
    TYPE_HOOK = 5,
    TYPE_CLIPDATA = 6  ,              //clipboard data
    TYPE_CALLPROC = 7,
    TYPE_ACCELTABLE = 8,
    TYPE_DDEACCESS = 9,
    TYPE_DDECONV = 10,
    TYPE_DDEXACT = 11,          //DDE transaction tracking info.
    TYPE_MONITOR = 12,
    TYPE_KBDLAYOUT = 13   ,           //Keyboard Layout handle (HKL) object.
    TYPE_KBDFILE = 14    ,            //Keyboard Layout file object.
    TYPE_WINEVENTHOOK = 15  ,         //WinEvent hook (EVENTHOOK)
    TYPE_TIMER = 16,
    TYPE_INPUTCONTEXT = 17  ,         //Input Context info structure
    TYPE_CTYPES = 18         ,        //Count of TYPEs; Must be LAST + 1
    TYPE_GENERIC = 255               //used for generic handle validation
} HANDLE_TYPE;

typedef struct _HANDLEENTRY {
    PVOID   phead;                      //pointer to the real object
    ULONG   pOwner;                     //pointer to owning entity (pti or ppi)
    BYTE    bType;                      //type of object
    BYTE    bFlags;                     //flags - like destroy flag
    short   wUniq;                      //uniqueness count
} HANDLEENTRY, * PHANDLEENTRY ;

typedef struct SERVERINFO { 	        //si
    short   wRIPFlags ;                 //RIPF_ flags
    short   wSRVIFlags ;                //SRVIF_ flags
    short   wRIPPID ;                   //PID of process to apply RIP flags to (zero means all)
    short   wRIPError ;                 //Error to break on (zero means all errors are treated equal)
    ULONG   cHandleEntries;             //count of handle entries in array
} SERVERINFO, * PSERVERINFO;

typedef struct SHAREDINFO {
    PSERVERINFO     psi;                //tagSERVERINFO
    PHANDLEENTRY    aheList;            //_HANDLEENTRY - handle table pointer
    ULONG   pDispInfo;                  //global displayinfo
    ULONG   ulSharedDelta;              //delta between client and kernel mapping of ...
}SHAREDINFO, * PSHAREDINFO;

typedef struct HEAD {
    HANDLE h;
    ULONG cLockObj;
}HEAD;

typedef struct THROBJHEAD {
    HEAD headinfo;
    PVOID pti; //PTHREADINFO
}THROBJHEAD;

typedef  struct DESKHEAD {
    PVOID rpdesk; //PDESKTOP
    PBYTE pSelf ; //PBYTE
} DESKHEAD;

typedef struct THRDESKHEAD {
    THROBJHEAD ThreadObjHead ;
    DESKHEAD DesktopHead ;
}THRDESKHEAD;

typedef struct _HOOK_INFO {
    HANDLE  hHook;                                //���ӵľ��
    DWORD   Unknown1;
    PVOID   Win32Thread;                            //һ��ָ�� win32k!_W32THREAD �ṹ���ָ��
    PVOID   Unknown2;
    PVOID   SelfHook;                               //ָ��ṹ����׵�ַ
    PVOID   NextHook;                               //ָ����һ�����ӽṹ��
    int     HookType;                              //���ӵ����ͣ� winuser.h ���ж���
    DWORD   OffPfn;                                 //���Ӻ����ĵ�ַƫ�ƣ����������ģ���ƫ��
    int     iHookFlags;
    int     iMod;                                   //���Ӻ�������ģ����������룬ͨ����ѯ WowProcess �ṹ���Եõ�ģ��Ļ���ַ��
    PVOID   Win32ThreadHooked;        
} HOOK_INFO, * PHOOK_INFO ;

typedef struct _MSG_HOOK_INFO {
    HANDLE  hHook ;
    INT     HookType;
    ULONG   EThread ;
    ULONG   offPfn ;
    PVOID   modBase;
    PVOID   funAdd;
    ULONG   ThreadId;
    ULONG   ProcessId;
} MSG_HOOK_INFO, * PMSG_HOOK_INFO ;

static WCHAR HookTypeString[16][32] = { 
    L"WH_MSGFILTER",
    L"WH_JOURNALRECORD",
    L"WH_JOURNALPLAYBACK", 
    L"WH_KEYBOARD", 
    L"WH_GETMESSAGE", 
    L"WH_CALLWNDPROC",
    L"WH_CBT", 
    L"WH_SYSMSGFILTER",
    L"WH_MOUSE", 
    L"WH_HARDWARE",
    L"WH_DEBUG", 
    L"WH_SHELL", 
    L"WH_FOREGROUNDIDLE", 
    L"WH_CALLWNDPROCRET",
    L"WH_KEYBOARD_LL", 
    L"WH_MOUSE_LL" 
};

#endif

