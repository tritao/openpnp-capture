#ifndef __KSJ_API_MULTI_PROCESS_H__
#define __KSJ_API_MULTI_PROCESS_H__

// #pragma message("Include KSJApiMultiProcess.h") 

#ifdef KSJAPI_EXPORTS
#define KSJ_API __declspec(dllexport)
#elif defined KSJAPI_STATIC
#define KSJ_API
#else
#define KSJ_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"{
#endif

	// ע�⣺��������˵��ʱ�����е�һ������ΪnIndex�ĺ��������豸����û�д���������£������Զ������豸���󲢴��豸��

	// �ر�ָ���������豸��������ر�ָ���������ô�ڶ�������ǲ��ܲ�����̨����ġ�
	// ��ȻKSJ_DeviceClose�����ĵ�һ������ΪnIndex�����ڲ�������ʽ�����豸���󲢿����豸��
	KSJ_API  int __stdcall KSJ_DeviceClose(int nIndex);

	enum KSJ_DEVICESTATUS { KSJ_DS_CLOSE, KSJ_DS_OPEN };
	// KSJ_DeviceGetStatus������������Զ������豸����ֻ�ǻ�ȡ��ǰ�豸�ڵ�ǰ���̵Ŀ���״̬��
	KSJ_API  int __stdcall KSJ_DeviceGetStatus(int nIndex, KSJ_DEVICESTATUS *pDs);



#ifdef __cplusplus
}
#endif

#endif