
#include "ssdt.h"


///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣���ȡSSDT���еķ������
//	�����������
//	�������������SSDT���еķ������
//
///////////////////////////////////////////////////////////////////////////////////
ULONG 
GetSsdtServiceNumber()
{
    KdPrint(("Enter GetSsdtServiceNumber"));
    return KeServiceDescriptorTable.NumberOfService;
}

///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣�ö��SSDT���еķ�������ַ
//	���������AddressInfo Ϊ����Ϊ PSSDT_INFO�����������ָ��,����ΪNULLʱ��Length�д�������Ҫ�Ļ��������ȣ�
//			 LengthΪ�������������ָ�룬�ں�������ʱ���Ĳ�������д�뻺���������ݳ���
//	�������������SSDT���еķ���������
//
///////////////////////////////////////////////////////////////////////////////////
ULONG 
GetSsdtCurrentAddresses (
    IN PSSDT_ADDRESS AddressInfo, 
    OUT IN PULONG Length
    )
{
    ULONG   ServiceNumber;
    ULONG   index;

    ServiceNumber = KeServiceDescriptorTable.NumberOfService;

    if (ServiceNumber * sizeof(SSDT_ADDRESS) > *Length)
    {
        *Length = ServiceNumber * sizeof(SSDT_ADDRESS);
        return 0;
    }

    if (AddressInfo == NULL)
    {
        *Length = ServiceNumber * sizeof(SSDT_ADDRESS);
        return 0;
    }

    for (index = 0; index < ServiceNumber; index++)
    {
        AddressInfo[index].nIndex = index;
        AddressInfo[index].FunAddress = (DWORD)KeServiceDescriptorTable.ServiceTableBase[index];
    }

    *Length = ServiceNumber * sizeof(SSDT_ADDRESS);
    return  ServiceNumber;
}

///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣� �޸�SSDT����ָ���ķ�������ַ,��������SSDT�������HOOK
//	��������� ServiceIndexΪҪ�޸ĵķ�������SSDT���е�������
//			  NewServiceAddress ΪҪ�޸ĵķ��������µ�ַ��
//	��������� ����ָ���ı��޸ķ�������ԭʼ��ַ
//
///////////////////////////////////////////////////////////////////////////////////
ULONG
SetServiceAddress(
    IN UINT     ServiceIndex,
    IN ULONG    NewServiceAddress
    )
{
    PMDL    MdlSystemCall = NULL;
    PVOID*  MappedPointer = NULL;
    PULONG  EnabledAddress = NULL;
    ULONG   OrigServiceAddress = 0;
    KIRQL   OrigIrql;
    
    KdPrint(("Enter SetServiceAddress"));
/*
    MdlSystemCall = MmCreateMdl(NULL, KeServiceDescriptorTable.ServiceTableBase, 
                                KeServiceDescriptorTable.NumberOfService * sizeof(ULONG)
                                );
*/
    MdlSystemCall = IoAllocateMdl(KeServiceDescriptorTable.ServiceTableBase,
                                  KeServiceDescriptorTable.NumberOfService * sizeof(ULONG),
                                  FALSE, FALSE, NULL);
    if (MdlSystemCall == NULL)
    {
        KdPrint(("[SetServiceAddress] MmCreateMdl failed"));
        return OrigServiceAddress;
    }

    MmBuildMdlForNonPagedPool(MdlSystemCall);
    MdlSystemCall->MdlFlags = MdlSystemCall->MdlFlags | MDL_MAPPED_TO_SYSTEM_VA;
    MappedPointer = MmMapLockedPagesSpecifyCache(MdlSystemCall, KernelMode, MmCached,
                                                 NULL, FALSE, NormalPagePriority );
    EnabledAddress = (PULONG)MappedPointer;

    KeRaiseIrql(DISPATCH_LEVEL, &OrigIrql);
    OrigServiceAddress = EnabledAddress[ServiceIndex];
    EnabledAddress[ServiceIndex] = NewServiceAddress;
    KeLowerIrql(OrigIrql);

    MmUnmapLockedPages(MappedPointer, MdlSystemCall);
    IoFreeMdl(MdlSystemCall);
    return OrigServiceAddress;
}

///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣��ָ�SSDT����ָ�������ķ������ĵ�ַ
//	���������SsdtInfo Ϊ SSDT_ADDRESS ���͵�ָ�룬������Ҫ�ָ��ķ����������͵�ַ
//	�������������TRUE��ʾ�ָ��ɹ������򷵻�FALSEʧ��
//
///////////////////////////////////////////////////////////////////////////////////
BOOLEAN
UnHookSsdtItem(
    IN PSSDT_ADDRESS SsdtInfo
    )
{
    if (SsdtInfo == NULL)
    {
        KdPrint(("[UnHookSsdtItem] ����ָ����Ч"));
        return FALSE;
    }

    if (SetServiceAddress(SsdtInfo->nIndex, SsdtInfo->FunAddress))
    {
        KdPrint(("[UnHookSsdtItem] �ָ� %d �ŷ���ɹ�", SsdtInfo->nIndex));
        return TRUE;
    }
    else
    {
        KdPrint(("[UnHookSsdtItem] SetServiceAddress failed"));
        return FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////

ULONG GetServiceIdByName(PCHAR FunctionName)
{
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    ULONG ServiceId = 0;
    UNICODE_STRING RootName;
    PUNICODE_STRING KernelLinkName = NULL;
    HANDLE FileHandle, SectionHandle;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK ioStatus;
    PVOID BaseAddress = NULL;
    size_t ViewSize = 0;
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNtHeader = NULL;
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;
    PIMAGE_EXPORT_DIRECTORY pExportDirectory = NULL;
    PULONG arrayOfFunctionNames = NULL;
    PULONG arrayOfFunctionAddresses = NULL;
    PWORD  arrayOfFunctionOrdinals = NULL;
    ULONG  i, funcOrdinal, funcAddress, number;
    PCHAR  funcName = NULL;

    RtlInitUnicodeString(&RootName, L"\\SystemRoot");
    KernelLinkName = Convert2KernelLinkName(&RootName);
    if (KernelLinkName == NULL)  return 0;
    
    ntStatus = RtlAppendUnicodeToString(KernelLinkName, L"\\system32\\ntdll.dll");
    if (!NT_SUCCESS(ntStatus)) {
        ExFreePoolWithTag(KernelLinkName, MEM_TAG);
        return 0;
    }
    InitializeObjectAttributes(&ObjectAttributes, KernelLinkName, 
                               OBJ_CASE_INSENSITIVE, 0, 0);

    ntStatus = ZwOpenFile(&FileHandle, FILE_EXECUTE | SYNCHRONIZE, 
                          &ObjectAttributes, &ioStatus, 
                          FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT);
    if (!NT_SUCCESS(ntStatus))  return 0;

    ObjectAttributes.ObjectName = NULL;

    ntStatus = ZwCreateSection(&SectionHandle, SECTION_ALL_ACCESS, 
                               &ObjectAttributes, 0, 
                               PAGE_EXECUTE, SEC_IMAGE, FileHandle);
    if (!NT_SUCCESS(ntStatus)) { 
        ZwClose(FileHandle); 
        return 0; 
    }
    ntStatus = ZwMapViewOfSection(SectionHandle, NtCurrentProcess(), 
                                  &BaseAddress, 0, 1024, 0, &ViewSize, 
                                  ViewShare, MEM_TOP_DOWN, PAGE_READWRITE);
    if (!NT_SUCCESS(ntStatus)) { 
        ZwClose(SectionHandle); 
        ZwClose(FileHandle); 
        return 0; 
    }
    pDosHeader = (PIMAGE_DOS_HEADER)BaseAddress;
    pNtHeader  = (PIMAGE_NT_HEADERS)((ULONG)BaseAddress + pDosHeader->e_lfanew);
    pOptionalHeader = &(pNtHeader->OptionalHeader);
    pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress + (ULONG)BaseAddress);

    arrayOfFunctionNames = (PULONG)(pExportDirectory->AddressOfNames + (ULONG)BaseAddress);
    arrayOfFunctionAddresses = (PULONG)(pExportDirectory->AddressOfFunctions + (ULONG)BaseAddress);
    arrayOfFunctionOrdinals = (PWORD)(pExportDirectory->AddressOfNameOrdinals + (ULONG)BaseAddress);

    for (i = 0; i < pExportDirectory->NumberOfNames; i++) {

        funcName = (PCHAR)(arrayOfFunctionNames[i] + (PCHAR)BaseAddress);
        funcOrdinal = arrayOfFunctionOrdinals[i] + pExportDirectory->Base - 1;
        funcAddress = (ULONG)(arrayOfFunctionAddresses[funcOrdinal] + (PCHAR)BaseAddress);

        if (funcName[0] == 'N' && funcName[1] == 't') {

            number = *((PULONG)(funcAddress + 1));
            if (*(PBYTE)funcAddress != MOV_OPCODE)  continue;
            if (number > pExportDirectory->NumberOfNames)  continue;

            if (strstr(funcName, FunctionName) != 0) {

                ServiceId = number;  break;
            }
        }
    }
    ZwUnmapViewOfSection(NtCurrentProcess(), BaseAddress);
    ZwClose(SectionHandle);
    ZwClose(FileHandle);
    ExFreePoolWithTag(KernelLinkName, MEM_TAG);
    return ServiceId;
}

////////////////////// End of File ///////////////////////////////////
