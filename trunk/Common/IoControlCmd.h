
#ifndef _IOCONTROLCMD_H
#define _IOCONTROLCMD_H

//////////////////////////////////////////////////////////////////////////

#define CDO_FLAG_FILE			1L
#define CDO_FLAG_DIRECTORY		2L

//
// Values defined for "Method"
// METHOD_BUFFERED
// METHOD_IN_DIRECT
// METHOD_OUT_DIRECT
// METHOD_NEITHER
// 
// Values defined for "Access"
// FILE_ANY_ACCESS
// FILE_READ_ACCESS
// FILE_WRITE_ACCESS

#define FILE_DEVICE_SEDECTIVE   0x8008
#define IOCTL_BASE              0x800
#define TEMPLATE_CTL_CODE(i)    (ULONG) \
    CTL_CODE(FILE_DEVICE_SEDECTIVE, IOCTL_BASE + i, METHOD_BUFFERED, FILE_ANY_ACCESS)


// ��ȡ ssdt �ṹ
#define IOCTL_GET_SSDT                  TEMPLATE_CTL_CODE(0x01)
// �ָ� ssdt
#define IOCTL_UNHOOK_SSDT               TEMPLATE_CTL_CODE(0x02)
// ��ȡ Shadow ssdt �ṹ
#define IOCTL_GET_SSDTSHADOW            TEMPLATE_CTL_CODE(0x03)
// �ָ� Shadow ssdt
#define IOCTL_UNHOOK_SSDTSHADOW         TEMPLATE_CTL_CODE(0x04)
// ���� PspCidTable ö�ٽ��� && ���������ڴ��ȡ���� 
#define IOCTL_GET_PROCESSES             TEMPLATE_CTL_CODE(0x06)
// ����EPROCESS��ȡ����dos·��
#define IOCTL_GET_PROCESS_IMAGE_PATH    TEMPLATE_CTL_CODE(0x07)
// �鿴���̼���ģ����Ϣ
#define IOCTL_GET_PROCESS_MODULES       TEMPLATE_CTL_CODE(0x08)
// �鿴�����߳���Ϣ, ��Ҫ������������
#define IOCTL_GET_PROCESS_THREADS       TEMPLATE_CTL_CODE(0x09)
// ��ȡ���̾��
#define IOCTL_GET_PROCESS_HANDLES       TEMPLATE_CTL_CODE(0x0A)
// ��ȡ�ں�ģ����Ϣ
#define IOCTL_GET_DRIVER_OBJECT         TEMPLATE_CTL_CODE(0x11)
// ��ȡ�ں˵�ַ����
#define IOCTL_DUMP_KERNEL_MEMORY        TEMPLATE_CTL_CODE(0x12)
// ���ұ���ģ��
#define IOCTL_PROTECT_MYSELF            TEMPLATE_CTL_CODE(0x13)
// �˳�ʱ����ģ��
#define IOCTL_EXIT_PROCESS              TEMPLATE_CTL_CODE(0x14)
// ��ȡָ��Ŀ¼���ļ���Ϣ������Ŀ¼
#define IOCTL_LIST_DIRECTORY            TEMPLATE_CTL_CODE(0x15)

//////////////////////////////////////////////////////////////////////////

#endif

