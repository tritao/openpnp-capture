#ifndef KSJ_COM_H
#define KSJ_COM_H

#ifdef KSJCOM_EXPORTS
#    pragma message("export dynamic ksjcom lib")
#    define KSJCOM_API __declspec(dllexport) int WINAPI
#elif defined( KSJCOM_STATIC )
#    pragma message("export or import static ksjcom lib")
#    define KSJCOM_API int WINAPI
#else
#    pragma message("import dynamic ksjcom lib")
#    define KSJCOM_API __declspec(dllimport) int WINAPI
#endif

#ifdef __cplusplus
extern "C"{
#endif

#define KSJCOM_RET_SUCCESS    0
#define KSJCOM_RET_FAIL       -1
#define KSJCOM_RET_DLLNOTINIT -2
#define KSJCOM_RET_OPENED     -3    // ��ʾ�˿��Ѿ��򿪣������ظ���

enum PROTOCOLTYPE 
{ 
	ADR8060_PROTOCOL   = 0, 
	PELCOD_PROTOCOL    = 1, 
	ZHANGSHAN_PROTOCOL = 2,
};

// ��ʼ��KSJCom��̬��
KSJCOM_API  KSJCOM_Init( void );

// ����ʼ��KSJCom��̬��
KSJCOM_API  KSJCOM_UnInit( void );

// �õ�COM�˿���Ŀ
KSJCOM_API  KSJCOM_GetCount( void );

// �õ�COM�˿ڵ����֣�������COM1,...
KSJCOM_API  KSJCOM_GetInfo( IN int nChannel, TCHAR szComName[256] );

// ����Ӧ��COM��
// nStopBits 0 - 1 stop bit; 1 - 1.5 stop bits; 2 - 2 stop bits
// nParity 0-4=None,Odd,Even,Mark,Space 
KSJCOM_API  KSJCOM_Open(int nChannel, int nBaud = 4800, BYTE nByteSize = 8, BYTE nStopBits = 0, BYTE nParity = 0);

// ����nBytesNum���ֽڵ����ݣ�pbtData�ɵ����߷��䲢�����ͷ�
// ���سɹ����͵��ֽ���
KSJCOM_API  KSJCOM_SendData( int nChannel, BYTE *pbtData, int nBytesNum );

KSJCOM_API  KSJCOM_SetTimeOut( int nChannel, DWORD dwReadTimeOut, DWORD dwWriteTimeOut );

// nBytesNumΪҪ��ȡ���ֽڸ��������nBytesNumС��ʵ�ʷ��͹������ֽڸ���������nBytesNum��ȡ��������ʵ�ʷ��͹������ֽڸ�����ȡ
// ����ʵ�ʶ�ȡ�����ֽڸ�����pbtData�ɵ����߷��䲢�����ͷ�
KSJCOM_API  KSJCOM_ReadData( int nChannel, BYTE *pbtData, int nBytesNum );

// �ر���Ӧ��COM��
KSJCOM_API  KSJCOM_Close( int nChannel );

// ��ָ����COM�������һ������������ָ���˽������ĵ�ַ��ʹ�õ�Э��
// ע�⣬ͬһ��COM���ϲ�Ҫ���������ַ��ͬ�Ľ�������
KSJCOM_API  KSJCOM_AddOneDecoder( int nChannel, BYTE btAddress, PROTOCOLTYPE protocolType );

KSJCOM_API  KSJCOM_Led254Off  ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_Led254On   ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_Led365Off  ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_Led365On   ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_Led300Off  ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_Led300On   ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_LedWhiteOff( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_LedWhiteOn ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_IrisClose  ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_IrisOpen   ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_FocusNear  ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_FocusFar   ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_ZoomOut    ( int nChannel, BYTE btAddress );
KSJCOM_API  KSJCOM_ZoomIn     ( int nChannel, BYTE btAddress );

KSJCOM_API  KSJCOM_StopCamera( int nChannel, BYTE btAddress );

// �������
// nIndex ��ʾ�����ĸ������nPosition ��ʾ�����ֵ
KSJCOM_API  KSJCOM_DuojiControl( int nChannel, int nIndex, int nPosition );


#ifdef __cplusplus
}
#endif

#endif

