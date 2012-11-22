
//#include "../../stdafx.h"
// ʹ��Ԥ����ͷ
#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣� ��ȡ�������ڵ��ں�ģ���·��
//	��������� dwFunAddressΪҪ��������ģ��ĺ�����ַ��
//			  cbNameΪpszName���ܳ��ȣ�
//			  bFlagΪTRUE,���Ƿ���ģ��ȫ·����ΪFALSE�򷵻�ģ������
//	��������� �Ƿ���óɹ�
//
///////////////////////////////////////////////////////////////////////////////////
BOOL 
GetKernelModuleNameByAddress(PSTR pszName, ULONG cbName, DWORD dwFunAddress, BOOL bFlag)
{
    NTSTATUS Status;
    PSYSTEM_MODULE_INFORMATION  Modules;
    PSYSTEM_MODULE_INFORMATION_ENTRY ModuleInfo;
    PVOID Buffer;
    ULONG BufferSize = 4096;
    ULONG ReturnLength;
    ULONG NameLen;

    if (dwFunAddress < 0x80000000)  return FALSE;

retry:
    Buffer = GlobalAlloc(GPTR, BufferSize);

    if (!Buffer)  return FALSE;

    Status = ZwQuerySystemInformation(SystemModuleInformation, Buffer,
                                      BufferSize, &ReturnLength);
    if (Status == STATUS_INFO_LENGTH_MISMATCH)
    {
        GlobalFree(Buffer);
        BufferSize = ReturnLength;
        goto retry;
    }

    if (NT_SUCCESS(Status))
    {
        Modules = (PSYSTEM_MODULE_INFORMATION)Buffer;
        ModuleInfo = &(Modules->Module[0]);

        for (ULONG i = 0; i < Modules->NumberOfModules; i ++, ModuleInfo ++)
        {
            if (dwFunAddress > (DWORD)(ModuleInfo->Base) &&
                dwFunAddress < (DWORD)(ModuleInfo->Base) + ModuleInfo->Size)
            {
                if (bFlag) {
                    NameLen = lstrlenA(ModuleInfo->FullPathName);
                    if (cbName < NameLen)  goto theEnd;
                    strncpy(pszName, ModuleInfo->FullPathName, NameLen);
                } else {
                    NameLen = lstrlenA(ModuleInfo->OffsetToFileName + ModuleInfo->FullPathName);
                    if (cbName < NameLen)  goto theEnd;
                    strncpy(pszName, ModuleInfo->OffsetToFileName + ModuleInfo->FullPathName, NameLen);
                }
                GlobalFree(Buffer);
                return TRUE;
            }
        }
    }
theEnd:
    GlobalFree(Buffer);
    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣� �����ں�ģ�����ƻ�ȡ�ں�ģ��ļ��ص�ַ
//	��������� pszName Ϊ�����ں�ģ������
//            pBase Ϊ�����ں�ģ����ص�ַ
//	��������� �Ƿ���óɹ�
//
///////////////////////////////////////////////////////////////////////////////////
BOOL 
GetKernelModuleBaseByName(PSTR pszName, PULONG SysBase)
{
    NTSTATUS Status;
    PSYSTEM_MODULE_INFORMATION  Modules;
    PSYSTEM_MODULE_INFORMATION_ENTRY ModuleInfo;
    PVOID Buffer;
    ULONG BufferSize = 4096;
    ULONG ReturnLength;

    if (SysBase == NULL)   return FALSE;

retry:
    Buffer = GlobalAlloc(GPTR, BufferSize);

    if (!Buffer)  return FALSE;

    Status = ZwQuerySystemInformation(SystemModuleInformation,
                                      Buffer,
                                      BufferSize,
                                      &ReturnLength
                                      );
    if (Status == STATUS_INFO_LENGTH_MISMATCH)
    {
        GlobalFree(Buffer);
        BufferSize = ReturnLength;
        goto retry;
    }

    if (NT_SUCCESS(Status))
    {
        Modules = (PSYSTEM_MODULE_INFORMATION)Buffer;
        ModuleInfo = &(Modules->Module[0]);

        for (ULONG i = 0; i < Modules->NumberOfModules; i ++, ModuleInfo ++)
        {
            if (_stricmp(ModuleInfo->OffsetToFileName + ModuleInfo->FullPathName, pszName) == 0)
            {
                *SysBase = (ULONG)(ModuleInfo->Base);
                GlobalFree(Buffer);
                return TRUE;
            }
        }
    }

    GlobalFree(Buffer);
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣� ��ȡ�������ڵ��ں�ģ���·��
//	��������� pszKernelName Ϊ�����ں�����
//            cbNameΪpszName���ܳ��ȣ�
//	��������� �Ƿ���óɹ�
//
///////////////////////////////////////////////////////////////////////////////////
BOOL 
GetKernelInformation(PSTR pszKernelName, ULONG cbName, PULONG KernelBase)
{
    NTSTATUS Status;
    PSYSTEM_MODULE_INFORMATION  Modules;
    PSYSTEM_MODULE_INFORMATION_ENTRY ModuleInfo;
    PVOID Buffer;
    ULONG BufferSize = 4096;
    ULONG ReturnLength;
    ULONG NameLen;

retry:
    Buffer = GlobalAlloc(GPTR, BufferSize);

    if (!Buffer)  return FALSE;

    Status = ZwQuerySystemInformation(SystemModuleInformation, Buffer, 
                                      BufferSize, &ReturnLength);
    if (Status == STATUS_INFO_LENGTH_MISMATCH)
    {
        GlobalFree(Buffer);
        BufferSize = ReturnLength;
        goto retry;
    }

    if (NT_SUCCESS(Status))
    {
        Modules = (PSYSTEM_MODULE_INFORMATION)Buffer;
        ModuleInfo = &(Modules->Module[0]);
        if (KernelBase != NULL)  KernelBase[0] = (DWORD)ModuleInfo->Base;
        NameLen = lstrlenA(ModuleInfo->OffsetToFileName + ModuleInfo->FullPathName);
        if (cbName < NameLen)  goto theEnd;
        strncpy(pszKernelName, ModuleInfo->OffsetToFileName + ModuleInfo->FullPathName, NameLen);
        GlobalFree(Buffer);
        return TRUE;
    }

theEnd:
    GlobalFree(Buffer);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////
BOOL 
GetPsModuleNameByAddress(
            ULONG  ProcessId, 
            ULONG pfnAddress, 
            LPTSTR pszModuleName, 
            ULONG cbszModuleName
            )
{
    MODULEENTRY32 ModuleEntry;
    HANDLE hSnapShot;
    BOOL bFlag = FALSE;

    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);
    ModuleEntry.dwSize = sizeof(MODULEENTRY32);
    bFlag = Module32First(hSnapShot, &ModuleEntry);
    while (bFlag) 
    {
        if ((pfnAddress >= (ULONG)ModuleEntry.modBaseAddr) &&
            (pfnAddress <= (ULONG)ModuleEntry.modBaseAddr + ModuleEntry.modBaseSize)) 
        {
            wcscpy_s(pszModuleName, cbszModuleName, ModuleEntry.szModule);
            CloseHandle(hSnapShot);
            return TRUE;
        } 
        bFlag = Module32Next(hSnapShot, &ModuleEntry);
    }
    CloseHandle(hSnapShot);
    return FALSE;
}