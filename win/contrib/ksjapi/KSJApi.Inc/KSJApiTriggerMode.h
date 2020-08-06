#ifndef __KSJ_API_TRIGGER_MODE_H__
#define __KSJ_API_TRIGGER_MODE_H__

// #pragma message("Include KSJApiTriggerMode.h") 

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

	// Trigger Mode
	enum KSJ_TRIGGERMODE
	{
		KSJ_TRIGGER_INTERNAL,
		KSJ_TRIGGER_EXTERNAL,
		KSJ_TRIGGER_SOFTWARE,
		KSJ_TRIGGER_FIXFRAMERATE,
		KSJ_TRIGGER_HIGHLOWFIXFRAMERATE
	};

	// Set Trigger Mode
	KSJ_API  int __stdcall KSJ_TriggerModeSet(int nChannel, KSJ_TRIGGERMODE TriggerMode);
	KSJ_API  int __stdcall KSJ_TriggerModeGet(int nChannel, KSJ_TRIGGERMODE *pTriggerMode);

	enum KSJ_TRIGGERSTATUS
	{
		KSJ_NO,
		KSJ_HAVE
	};

	// Get Frame Buffer Status. Has One or more Frame or None. Maybe return RET_NOTSUPPORT.
	// When Fpga Version is not 0.0, This Function should support.
	KSJ_API  int __stdcall KSJ_TriggerStatusGet(int nChannel, KSJ_TRIGGERSTATUS *pTriggerStatus);

	// Empty Frame Buffer. Not Support When In KSJ_TRIGGER_SOFTWARE Mode.
	KSJ_API  int __stdcall KSJ_EmptyFrameBuffer(int nChannel);

	// Same to call KSJ_EmptyFrameBuffer before KSJ_CaptureRawData. Not Support When In KSJ_TRIGGER_SOFTWARE Mode.
	KSJ_API  int __stdcall KSJ_CaptureRawDataAfterEmptyFrameBuffer(int nChannel, unsigned char *pRawData);
	// Same to call KSJ_EmptyFrameBuffer before KSJ_CaptureRgbData. Not Support When In KSJ_TRIGGER_SOFTWARE Mode.
	KSJ_API  int __stdcall KSJ_CaptureRgbDataAfterEmptyFrameBuffer(int nChannel, unsigned char *pRgbData);
	// Same to call KSJ_EmptyFrameBuffer before KSJ_CaptureRgbDataEx. Not Support When In KSJ_TRIGGER_SOFTWARE Mode.
	KSJ_API  int __stdcall KSJ_CaptureRgbDataExAfterEmptyFrameBuffer(int nChannel, unsigned char *pRgbData, int *pnWidth, int *pnHeight, int *pnBitCount);


	// Fixed Frame Rate when in KSJ_TRIGGER_FIXFRAMERATE/KSJ_TRIGGER_HIGHLOWFIXFRAMERATE mode. Maybe Return RET_NOTSUPPORT.
	KSJ_API  int __stdcall KSJ_SetFixedFrameRateEx(int nChannel, float fFrameRate);
	KSJ_API  int __stdcall KSJ_GetFixedFrameRateEx(int nChannel, float *pfFrameRate);

	enum KSJ_TRIGGERMETHOD
	{
		KSJ_TRIGGER_FALLINGEDGE,
		KSJ_TRIGGER_RISINGEDGE,
		KSJ_TRIGGER_HIGHLEVEL,
		KSJ_TRIGGER_LOWLEVEL
	};
	// Set Trigger Method when in KSJ_TRIGGER_EXTERNAL mode.
	KSJ_API  int __stdcall KSJ_TriggerMethodSet(int nChannel, KSJ_TRIGGERMETHOD  TriggerMethod);
	KSJ_API  int __stdcall KSJ_TriggerMethodGet(int nChannel, KSJ_TRIGGERMETHOD *pTriggerMethod);

	// Set Trigger Delay when in KSJ_TRIGGER_EXTERNAL. May be Return RET_NOTSUPPORT.
	KSJ_API  int __stdcall KSJ_TriggerDelayGetRange(int nChannel, unsigned short *wMin, unsigned short *wMax);
	KSJ_API  int __stdcall KSJ_TriggerDelaySet(int nChannel, unsigned short wDelay);
	KSJ_API  int __stdcall KSJ_TriggerDelayGet(int nChannel, unsigned short *pwDelay);

	// Set Read action timeout (ms). Influence capture and preview.
	KSJ_API  int __stdcall KSJ_CaptureSetTimeOut(int nChannel, unsigned long dwTimeOut);
	// Get Read action image timeout (ms).
	KSJ_API  int __stdcall KSJ_CaptureGetTimeOut(int nChannel, unsigned long *pdwTimeOut);

	// Set If recover device when Read Fail. It will Influence capture and preview.
	KSJ_API  int __stdcall KSJ_CaptureSetRecover(int nChannel, bool bRecover);
	// Get If recover device when Read Fail.
	KSJ_API  int __stdcall KSJ_CaptureGetRecover(int nChannel, bool *pbRecover);

	// Set timeout and recover at the same time
	KSJ_API  int __stdcall KSJ_CaptureSetTimeOutEx(int nChannel, unsigned long dwTimeOut, bool bRecover);
	// Get timeout and recover at the same time
	KSJ_API  int __stdcall KSJ_CaptureGetTimeOutEx(int nChannel, unsigned long *pdwTimeOut, bool *pbRecover);

#ifdef __cplusplus
}
#endif

#endif