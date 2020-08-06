#ifndef __KSJ_API_CUSTOM_H__
#define __KSJ_API_CUSTOM_H__

// #pragma message("Include KSJApiCustom.h") 

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

	// ��pData(���ΪnWidth, �߶�ΪnHeight, ����λ��ΪnBitCount)��ͼ����������Ϊ���ΪnStretchWidh, �߶�ΪnStretchHeight�����ݲ���䵽�û������pStretchData������,����λ������
	KSJ_API  int __stdcall KSJ_HelperStretchBmp(unsigned char *pData, int nWidth, int nHeight, int nBitCount, unsigned char *pStretchData, int nStretchWidth, int nStretchHeight);
	// ����ͼ��ı�����ȵõ�BMPͼ����ļ�ͷ,��Ϣͷ,����ɫ���ܵ��ֽ���Ŀ
	KSJ_API  int __stdcall KSJ_HelperGetBmpFileHeaderSize(int nBitCount, int *pnTotalBytes);
	// �õ�BMPͼ����ļ�ͷ����Ϣͷ
	KSJ_API  int __stdcall KSJ_HelperGetBmpFileHeader(int nWidth, int nHeight, int nBitCount, void *pHeader);
	// �ɼ�һ֡ͼ��ϵͳ������
	KSJ_API  int __stdcall KSJ_CaptureRgbDataToClipboard(int nChannel);

	// ����FFC�ӳ�
	KSJ_API  int __stdcall KSJ_SetFlatFieldCorrectionFieldOfView(int nChannel, int nColStart, int nRowStart, int nColSize, int nRowSize, KSJ_ADDRESSMODE AmCol, KSJ_ADDRESSMODE AmRow, unsigned short wMultiFrameNum);

	// ��ȡFFC���ӳ����ӳ�������ͬ�ſ��Խ���У����
	KSJ_API  int __stdcall KSJ_GetFlatFieldCorrectionFieldOfView(int nChannel, int *pnColStart, int *pnRowStart, int *pnColSize, int *pnRowSize, KSJ_ADDRESSMODE *pColAddressMode, KSJ_ADDRESSMODE *pRowAddressMode, unsigned short *pwMultiFrameNum);

	// ���ݲɼ��ӳ��ɼ�nDarkFrames��ͼ��ƽ������ΪDarkFrame
	KSJ_API  int __stdcall KSJ_MakeDarkFrame(int nChannel, int nDarkFrames);
	// ���ݲɼ��ӳ��ɼ�nFlatFieldFrames��ͼ��ƽ������ΪFlatFieldImage
	KSJ_API  int __stdcall KSJ_MakeFlatFieldFrame(int nChannel, int nFlatFieldFrames);

	// Ԥ��ʱ�Ƿ�ʹ��
	KSJ_API  int __stdcall KSJ_PreviewSetFlatFieldCorrection(int nChannel, bool bEnable);

	KSJ_API  int __stdcall KSJ_PreviewGetFlatFieldCorrection(int nChannel, bool *pbEnable);
	// �ɼ�ʱ�Ƿ�ʹ��
	KSJ_API  int __stdcall KSJ_CaptureSetFlatFieldCorrection(int nChannel, bool bEnable);

	KSJ_API  int __stdcall KSJ_CaptureGetFlatFieldCorrection(int nChannel, bool *pbEnable);

	KSJ_API  int __stdcall KSJ_SaveFlatFieldCorrection(int nChannel, TCHAR *pszFilePath);

	KSJ_API  int __stdcall KSJ_LoadFlatFieldCorrection(int nChannel, TCHAR *pszFilePath);


	// Ԥ��ʱ�Ƿ�ʹ��
	KSJ_API  int __stdcall KSJ_PreviewSetCalibration(int nChannel, bool bEnable);

	KSJ_API  int __stdcall KSJ_PreviewGetCalibration(int nChannel, bool *pbEnable);
	// �ɼ�ʱ�Ƿ�ʹ��
	KSJ_API  int __stdcall KSJ_CaptureSetCalibration(int nChannel, bool bEnable);

	KSJ_API  int __stdcall KSJ_CaptureGetCalibration(int nChannel, bool *pbEnable);

	KSJ_API  int __stdcall KSJ_LoadCalibrationMapFile(int nChannel, const TCHAR *pszFilePath);

	enum KSJ_MAPMODE 
	{
		KSJ_MM_NEARESTNEIGHBOR,
		KSJ_MM_BILINEAR
	};

	KSJ_API  int __stdcall KSJ_SetCalibrationMapMode(int nChannel, KSJ_MAPMODE MapMode);
	KSJ_API  int __stdcall KSJ_GetCalibrationMapMode(int nChannel, KSJ_MAPMODE *pMapMode);

#ifdef __cplusplus
}
#endif

#endif
