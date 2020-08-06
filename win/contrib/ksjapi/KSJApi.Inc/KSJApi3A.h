#ifndef __KSJ_API_3A_H__
#define __KSJ_API_3A_H__

// #pragma message("Include KSJApi3A.h") 

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

	// �����Զ��ع������������������Ԥ��ͼ������Ͻ�����λ��
	KSJ_API  int __stdcall KSJ_AESetRegion(int nChannel, int nX, int nY, int nW, int nH);
	// Get AE Region (according to preview image left top coner ) and Show Status.
	KSJ_API  int __stdcall KSJ_AEGetRegion(int nChannel, int *pnX, int *pnY, int *pnW, int *pnH, bool *pbShow);
	// �����Զ��ع������������������Ԥ��ͼ������Ͻ�����λ��,�Ƿ���ʾ����
	KSJ_API  int __stdcall KSJ_AEShowRegion(int nChannel, bool bShow);

	KSJ_API  int __stdcall KSJ_AESetPeakAveRatio(int nChannel, float fRatio);
	KSJ_API  int __stdcall KSJ_AEGetPeakAveRatio(int nChannel, float *pfRatio);

	// Set Exposure Time Range When Do Auto Exposure
	KSJ_API  int __stdcall KSJ_AESetExposureTimeRange(int nChannel, float fMinExpMs,   float fMaxExpMs);
	KSJ_API  int __stdcall KSJ_AEGetExposureTimeRange(int nChannel, float *pfMinExpMs, float *pfMaxExpMs);

	// AE Status return to callback
	enum KSJ_AE_STATUS { KSJ_AE_ONCE_SUCCESS = 0, KSJ_AE_ONCE_FAIL_MAX_COUNT, KSJ_AE_ONCE_FAIL_WAVE, KSJ_AE_ONCE_FAIL_OVER_RANGE, KSJ_AE_DOING, KSJ_AE_CONTINUE_SUCCESS, KSJ_AE_CONTINUE_FAIL_WAVE, KSJ_AE_CONTINUE_FAIL_OVER_RANGE };
	// Auto Exposure Ex Callback, Call when AE Doing, Success, Fail.
	typedef void(__stdcall *KSJ_AECALLBACKEX)( KSJ_AE_STATUS Status, int nExpsoureTimeMs, void *lpContext);
	KSJ_API  int __stdcall KSJ_AESetCallbackEx(int nChannel, KSJ_AECALLBACKEX pfAECallbackEx, void *lpContext);

	KSJ_API  int __stdcall KSJ_AESetMaxCount(int nChannel, int  nMaxCount);
	KSJ_API  int __stdcall KSJ_AESetTarget  (int nChannel, int  nTarget  );
	KSJ_API  int __stdcall KSJ_AEStartEx    (int nChannel, bool bStart   );
	KSJ_API  int __stdcall KSJ_AEGetStatusEx(int nChannel, bool *pbStart, int *pnMaxCount, int *pnTarget);

	// Auto Exposure Callback, Call when AE Finish.
	typedef void(__stdcall *KSJ_AECALLBACK)(bool bSuccess, int nResult, void *lpContext);
	// �����Զ��ع����������Ļص����������Զ��ع����ʱ��KSJ���������Զ�����AECallback�û�ָ���Ļص�����
	KSJ_API  int __stdcall KSJ_AESetCallback(int nChannel, KSJ_AECALLBACK pfAECallback, void *lpContext);
	// ��ʼ����ֹ�Զ��ع����
	KSJ_API  int __stdcall KSJ_AEStart(int nChannel, bool bStart, int nMaxCount, int nTarget);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �����Զ���ƽ������������������Ԥ��ͼ������Ͻ�����λ��
	KSJ_API  int __stdcall KSJ_AWBSetRegion(int nChannel, int nX, int nY, int nW, int nH);
	// �����Զ���ƽ������������������Ԥ��ͼ������Ͻ�����λ��,�Ƿ���ʾ����
	KSJ_API  int __stdcall KSJ_AWBShowRegion(int nChannel, bool bShow);
	KSJ_API  int __stdcall KSJ_AWBGetRegion(int nChannel, int *pnX, int *pnY, int *pnW, int *pnH, bool *pbShow);

	// �Զ���ƽ��ص�����
	typedef void(__stdcall *KSJ_AWBCALLBACK)(bool bSuccess, int nValueR, int nValueG, int nValueB, void *lpContext);
	// �����Զ���ƽ�⣨����Ӳ��������������Ļص����������Զ��ع����ʱ��KSJ���������Զ�����AECallback�û�ָ���Ļص�����
	KSJ_API  int __stdcall KSJ_AWBSetCallback(int nChannel, KSJ_AWBCALLBACK fpAWBCallback, void *lpContext);
	// ��ʼ����ֹ�Զ���ƽ�����
	KSJ_API  int __stdcall KSJ_AWBStart(int nChannel, bool bStart, int nMaxCount, int nRTarget, int nGTarget, int nBTarget);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���þ۽���������
	KSJ_API  int __stdcall KSJ_AFSetRegion(int nChannel, int nX, int nY, int nW, int nH);
	KSJ_API  int __stdcall KSJ_AFShowRegion(int nChannel, bool bShow);
	KSJ_API  int __stdcall KSJ_AFGetRegion(int nChannel, int *pnX, int *pnY, int *pnW, int *pnH, bool *pbShow);
	// �۽������ص�����, nValueΪ���ص�����ֵ
	typedef void(__stdcall *KSJ_AFCALLBACK)(int nValue, void *lpContext);
	// ���þ۽������ص�
	KSJ_API  int __stdcall KSJ_AFSetCallback(int nChannel, KSJ_AFCALLBACK fpAFCallback, void *lpContext);
	// ��ʼ����ֹ�۽�����
	KSJ_API  int __stdcall KSJ_AFStart(int nChannel, bool bStart);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ӳ���Զ��ع�
	KSJ_API  int __stdcall KSJ_AECSetEnable ( int nChannel, bool bEnable  );
	KSJ_API  int __stdcall KSJ_AECGetEnable ( int nChannel, bool *pbEnable  );

	KSJ_API  int __stdcall KSJ_AECSetRange ( int nChannel, unsigned short usMin,  unsigned short usMax  );
	KSJ_API  int __stdcall KSJ_AECGetRange ( int nChannel, unsigned short *pusMin, unsigned short *pusMax );
	// �õ��Զ��ع���ڵĵ�ǰ�ع�ֵ
	KSJ_API  int __stdcall KSJ_AECGetCur ( int nChannel, unsigned short *usCur  );

	// Ӳ���Զ�����
	KSJ_API  int __stdcall KSJ_AGCSetEnable ( int nChannel, bool bEnable  );
	KSJ_API  int __stdcall KSJ_AGCGetEnable ( int nChannel, bool *pbEnable  );

	KSJ_API  int __stdcall KSJ_AGCSetRange ( int nChannel, unsigned short usMin,  unsigned short usMax  );
	KSJ_API  int __stdcall KSJ_AGCGetRange ( int nChannel, unsigned short *pusMin, unsigned short *pusMax );

	// �õ��Զ�������ڵĵ�ǰ����ֵ
	KSJ_API  int __stdcall KSJ_AGCGetCur ( int nChannel, unsigned short *pusCur  );

#ifdef __cplusplus
}
#endif

#endif