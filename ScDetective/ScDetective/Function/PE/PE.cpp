
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣�����PE�ļ�����ȡPE�ṹ�и���ͷ��ָ��
//	���������ibaseΪӳ�����ַ��
//			  pfh,poh,psh�ֱ�ΪIMAGE_FILE_HEADER��IMAGE_OPTIONAL_HEADER��
//			  IMAGE_SECTION_HEADER��ָ��
//	�������������TRUE��ʾ��ȡ�ɹ�������ʧ��
//
///////////////////////////////////////////////////////////////////////////////////
DWORD GetHeaders(
    PCHAR ibase,
    PIMAGE_FILE_HEADER *pfh,
    PIMAGE_OPTIONAL_HEADER *poh,
    PIMAGE_SECTION_HEADER *psh
    )

{
    TRACE0("��ȡPE�ļ�ͷ��Ϣ");
    PIMAGE_DOS_HEADER mzhead=(PIMAGE_DOS_HEADER)ibase;

    if    ((mzhead->e_magic!=IMAGE_DOS_SIGNATURE) ||        
        (ibaseDD[mzhead->e_lfanew]!=IMAGE_NT_SIGNATURE))
        return FALSE;

    *pfh=(PIMAGE_FILE_HEADER)&ibase[mzhead->e_lfanew];
    if (((PIMAGE_NT_HEADERS)*pfh)->Signature!=IMAGE_NT_SIGNATURE) 
        return FALSE;
    *pfh=(PIMAGE_FILE_HEADER)((PBYTE)*pfh+sizeof(IMAGE_NT_SIGNATURE));

    *poh=(PIMAGE_OPTIONAL_HEADER)((PBYTE)*pfh+sizeof(IMAGE_FILE_HEADER));
    if ((*poh)->Magic!=IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        return FALSE;

    *psh=(PIMAGE_SECTION_HEADER)((PBYTE)*poh+sizeof(IMAGE_OPTIONAL_HEADER));
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣���ȡ�Լ���ģ���к�����ַ��RVA��
//	���������ModuleBaseΪģ����ص�ַ��
//			 pszFuncNameΪ��������
//           pFuncAddress Ϊ���������ַ��RVA��
//	����������Ƿ���ҳɹ�
//
///////////////////////////////////////////////////////////////////////////////////
BOOL 
GetFuncAddressFromIAT(HMODULE ModuleBase, PSTR pszFuncName, PULONG pFuncAddress)
{
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//
//	����ʵ�֣���ȡָ���ļ��е�ָ���ڵ�ָ��
//	���������ImagePathΪҪ��ȡָ����ָ���ӳ��·��
//			  SetionNameΪҪ���ҵĽ���
//			  ImageBaseΪ���ص�ӳ���Ĭ�ϼ��ػ���ַ;
//			  pFileΪ����ڴ�ӳ��Ļ�������ַ;
//	�������������ָ��ָ�������ݵ�ָ��
//
///////////////////////////////////////////////////////////////////////////////////
PCHAR 
GetSectionPoint(PCTSTR ImagePath, PCSTR SectionName, PDWORD ImageBase, char** pFile)
{
    HANDLE  hFile;
    DWORD   FileSize = 0;
    DWORD   ReadSize = 0;
    char*   BufferFile = NULL;

    hFile = CreateFile(ImagePath, GENERIC_READ, FILE_SHARE_READ,
                       NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)  return NULL;

    FileSize = GetFileSize(hFile, 0);
    
    BufferFile = (char*)GlobalAlloc(GPTR, FileSize);
    if (BufferFile == NULL) {
        CloseHandle(hFile);
        return NULL;
    }
    
    ReadFile(hFile, BufferFile, FileSize, &ReadSize, NULL);
    if (ReadSize <= 0) {
        GlobalFree(BufferFile);
        CloseHandle(hFile);
        return NULL;
    }
    CloseHandle(hFile);

    PIMAGE_FILE_HEADER      FileHeader;
    PIMAGE_OPTIONAL_HEADER  OptionalHeader;
    PIMAGE_SECTION_HEADER   SectionHeader;

    GetHeaders(BufferFile, &FileHeader, &OptionalHeader, &SectionHeader);  
    if (ImageBase != NULL)  *ImageBase = OptionalHeader->ImageBase;

    for (int i = 0; i < FileHeader->NumberOfSections; i ++, SectionHeader ++)  
    {
        if (_stricmp((char*)SectionHeader->Name, SectionName) == 0) 
        {
            if (SectionHeader->PointerToRawData == NULL) 
            {
                GlobalFree(BufferFile);
                return NULL;
            }
            break;
        }
    }

    *pFile = BufferFile;
    char* Buffer = (char*)(BufferFile + SectionHeader->PointerToRawData);
    return Buffer;
}