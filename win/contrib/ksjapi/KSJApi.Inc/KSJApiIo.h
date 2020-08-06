#ifndef __KSJ_API_IO_H__
#define __KSJ_API_IO_H__

// #pragma message("Include KSJApiIo.h") 

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

// KSJ_QueryFunction ( KSJ_SUPPORT_SNAP_BUTTON )
enum KSJ_SNAPBUTTONSTATUS 
{ 
	KSJ_UP, 
	KSJ_DOWN 
};
// Get Snap Pin Status
// When KSJ_SUPPORT_GPIO_NORMAL is True, KSJ_SUPPORT_SNAP_BUTTON will Enable after Set External Trigger Pin as Normal Input Pin.
KSJ_API  int __stdcall KSJ_SnapButtonStatus( int nChannel, KSJ_SNAPBUTTONSTATUS *pSnapButtonStatus );

// ����GPIO���ŵ��������״̬��btDirectionλѰַ��BIT0��ӦGPIO0, BIT1��ӦGPIO1, BIT2��ӦGPIO2, BIT3��ӦGPIO3(=1��ʾ��GPIO�����=0��ʾ��GPIO����)
KSJ_API  int __stdcall KSJ_GpioSetDirection( int nChannel, unsigned char btDirection );
// ��ȡGPIO���ŵ��������״̬��pbtDirectionλѰַ��BIT0��ӦGPIO0, BIT1��ӦGPIO1, BIT2��ӦGPIO2, BIT3��ӦGPIO3(=1��ʾ��GPIO�����=0��ʾ��GPIO����)
KSJ_API  int __stdcall KSJ_GpioGetDirection( int nChannel, unsigned char *pbtDirection );

// ����GPIO���������ֻ��btDirection��Ӧλ����Ϊ1�����Ų���ͨ���˺����������������btGpioStatusλѰַ��BIT0��ӦGPIO0, BIT1��ӦGPIO1, BIT2��ӦGPIO2, BIT3��ӦGPIO3(=1��ʾ��GPIO���TTL�ߵ�ƽ��=0��ʾ��GPIO���TTL�͵�ƽ)
KSJ_API  int __stdcall KSJ_GpioSetStatus   ( int nChannel, unsigned char btGpioStatus );
// ��ȡGPIO����״̬��*pbtGpioStatusλѰַ��BIT0��ӦGPIO0, BIT1��ӦGPIO1, BIT2��ӦGPIO2, BIT3��ӦGPIO3(=1��ʾ��GPIO���TTL�ߵ�ƽ��=0��ʾ��GPIO���TTL�͵�ƽ)
KSJ_API  int __stdcall KSJ_GpioGetStatus   ( int nChannel, unsigned char *pbtGpioStatus );

// About Flash. nMode not valid.
KSJ_API  int __stdcall KSJ_FlashControlSet(int nChannel, bool bEnable, bool bInvert, int nMode);
KSJ_API  int __stdcall KSJ_FlashControlGet(int nChannel, bool *pbEnable, bool *pbInvert, int *pnMode);

KSJ_API  int __stdcall KSJ_FlashEnableSet(int nChannel, bool bEnable);
KSJ_API  int __stdcall KSJ_FlashEnableGet(int nChannel, bool *pbEnable);

KSJ_API  int __stdcall KSJ_FlashInvertSet(int nChannel, bool bInvert);
KSJ_API  int __stdcall KSJ_FlashInvertGet(int nChannel, bool *pbInvert);

enum KSJ_FLASHMODE
{
	KSJ_FM_TROW,
	KSJ_FM_SIMULTANEOUSEXPOSURE,
	KSJ_FM_SHUTTERWIDTH
};
KSJ_API  int __stdcall KSJ_FlashSetMode(int nChannel, KSJ_FLASHMODE FlashMode);
KSJ_API  int __stdcall KSJ_FlashGetMode(int nChannel, KSJ_FLASHMODE *pFlashMode);

// ���º���ֻ������YYO�ͺ����(֡�������)
// ͨ����ѯ���ܵ������º���
// ע�⣺���ⴥ��ģʽ�£�����������ΪKSJ_GPIOIN_NORMAL�������л������ⴥ��ģʽʱ���á�
// ���û���������Ϊ�ⴥ��ģʽʱ���Զ�����ΪKSJ_GPIOIN_EXTERNALTRIGGER��
// KSJ_QueryFunction (KSJ_SUPPORT_GPIO_NORMAL)
// When call KSJ_GpioSetDirection(, bit0 = 0 ) == KSJ_GpioInModeSet(, KSJ_GPIOIN_NORMAL)
// When call KSJ_GpioSetDirection(, bit0 = 1 ) == KSJ_GpioInModeSet(, KSJ_GPIOIN_EXTERNALTRIGGER)
enum KSJ_GPIOIN_MODE 
{ 
	KSJ_GPIOIN_EXTERNALTRIGGER, 
	KSJ_GPIOIN_NORMAL
};
// �ⴥ����������Ĭ��Ϊ�ⴥ�����룬Ҳ��������Ϊ��ͨIO���룬ͨ����ȡ�������ж�ȡ��
KSJ_API  int __stdcall KSJ_GpioInModeSet(int nChannel, KSJ_GPIOIN_MODE GpioInMode);
KSJ_API  int __stdcall KSJ_GpioInModeGet(int nChannel, KSJ_GPIOIN_MODE *pGpioInMode);
// ���ⴥ������������Ϊ��ͨIOʱ������ͨ���˺������ж�ȡ��BIT0Ϊ�����ŵ�ƽ״̬��1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ��
// ע�⣺ֻ��������ΪKSJ_GPIOIN_NORMALģʽ�²ſ��Զ�ȡ��
KSJ_API  int __stdcall KSJ_GpioInStatusGet(int nChannel, unsigned char *pbtValue);

// ͨ����ѯKSJ_SUPPORT_GPIO_NORMAL���ܵ������º���
// When call KSJ_GpioSetDirection(, bit1 = 1 ) == KSJ_GpioOutModeSet(, KSJ_GPIOOUT_FLASH)
// When call KSJ_GpioSetDirection(, bit1 = 0 ) == KSJ_GpioOutModeSet(, KSJ_GPIOOUT_NORMAL)
// 
enum KSJ_GPIOOUT_MODE
{ 
	KSJ_GPIOOUT_FLASH, 
	KSJ_GPIOOUT_NORMAL 
};
KSJ_API  int __stdcall KSJ_GpioOutModeSet(int nChannel, KSJ_GPIOOUT_MODE GpioOutMode);
KSJ_API  int __stdcall KSJ_GpioOutModeGet(int nChannel, KSJ_GPIOOUT_MODE *pGpioOutMode);

// ע�⣺ֻ����KSJ_NORMAL_OUTģʽ�£��ſ������á�
KSJ_API  int __stdcall KSJ_GpioOutStatusSet(int nChannel, unsigned char btValue);

// �����˲�ʱ�䣬��λΪUs���������Ϊ0����Ӳ������200ns�����˲���
KSJ_API  int __stdcall KSJ_GpioFilterSet(int nChannel, unsigned short wFilterTimeUs);
KSJ_API  int __stdcall KSJ_GpioFilterGet(int nChannel, unsigned short *pwFilterTimeUs);



#ifdef __cplusplus
}
#endif


#endif