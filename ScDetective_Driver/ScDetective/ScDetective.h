
// $Id$

#ifndef __SCDETECT_H_VERSION__
#define __SCDETECT_H_VERSION__ 100

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <ntddk.h>
#include <string.h>
#include <ntstrsafe.h>
#include <windef.h>
#include "drvcommon.h"
#include "drvversion.h"

//////////////////////////////////////////////////////////////////////////
// ����һЩȫ�ֱ���
//////////////////////////////////////////////////////////////////////////
PEPROCESS   g_SystemProcess = NULL;
ULONG       g_SystemProcessId;
PEPROCESS   g_IdleProcess = NULL;
ULONG       g_IdleProcessId;
PEPROCESS   g_CsrssProcess = NULL;
ULONG       g_CsrssProcessId;
PEPROCESS   g_OwnerProcess = NULL;
ULONG       g_OwnerProcessId;

//
// This is only the address the handle table
// Remember [ HandleTable = *(PULONG)g_PspCidTable ] when you use it.
//
ULONG       g_PspCidTable = 0;
ULONG       g_PsActiveProcessHead = 0;
//////////////////////////////////////////////////////////////////////////

extern POBJECT_TYPE *IoDriverObjectType;
extern POBJECT_TYPE *PsProcessType;
extern POBJECT_TYPE *IoFileObjectType;

//////////////////////////////////////////////////////////////////////////
// ����ģ��
//////////////////////////////////////////////////////////////////////////
// һЩ���õĽṹ��
#include "e:\DriverStudy\struct.h"
// �û��㣬�ں˹���ģ��
#include "../../Common/IoControlCmd.h"
#include "../../Common/DataStruct.h"
// �ں�ģ���ʼ������
#include "System/Initialize.h"
// ssdt ����ģ��
#include "ssdt/ssdt.h"
// Shadow ssdt ����ģ��
#include "ssdt/ssdt_shadow.h"
// ����/ģ�� ��ع���ģ��
#include "Process/Process.h"
#include "Process/module.h"
// LDasm
#include "LDasm/LDasm.h"
// File
#include "File/File.h"
// Memory
#include "Memory/memory.h"
// HookEngine
#include "HookEngine/HookEngine.h"

#include "Protect/ScProtect.h"



#define DEVICE_NAME         "\\Device\\ScDetective"
#define SYMLINK_NAME        "\\DosDevices\\ScDetective"
PRESET_UNICODE_STRING(usDeviceName, DEVICE_NAME);
PRESET_UNICODE_STRING(usSymlinkName, SYMLINK_NAME);

#define MEM_TAG     'ScTt'

/*
#if DBG
#define KdPrint(_x_)  DbgPrint("[%08X] ScDetective", KeGetCurrentThread()); \
                      DbgPrint _x_
#else
#define KdPrint(_x_) 
#endif
*/
#endif // __SCDETECT_H_VERSION__
