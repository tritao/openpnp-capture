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
#define KSJCOM_RET_OPENED     -3    // 表示端口已经打开，不能重复打开

enum PROTOCOLTYPE 
{ 
	ADR8060_PROTOCOL   = 0, 
	PELCOD_PROTOCOL    = 1, 
	ZHANGSHAN_PROTOCOL = 2,
};

// 初始化KSJCom动态库
KSJCOM_API  KSJCOM_Init( void );

// 反初始化KSJCom动态库
KSJCOM_API  KSJCOM_UnInit( void );

// 得到COM端口数目
KSJCOM_API  KSJCOM_GetCount( void );

// 得到COM端口的名字，类似于COM1,...
KSJCOM_API  KSJCOM_GetInfo( IN int nChannel, TCHAR szComName[256] );

// 打开相应的COM口
// nStopBits 0 - 1 stop bit; 1 - 1.5 stop bits; 2 - 2 stop bits
// nParity 0-4=None,Odd,Even,Mark,Space 
KSJCOM_API  KSJCOM_Open(int nChannel, int nBaud = 4800, BYTE nByteSize = 8, BYTE nStopBits = 0, BYTE nParity = 0);

// 发送nBytesNum个字节的数据，pbtData由调用者分配并负责释放
// 返回成功发送的字节数
KSJCOM_API  KSJCOM_SendData( int nChannel, BYTE *pbtData, int nBytesNum );

KSJCOM_API  KSJCOM_SetTimeOut( int nChannel, DWORD dwReadTimeOut, DWORD dwWriteTimeOut );

// nBytesNum为要读取的字节个数，如果nBytesNum小于实际发送过来的字节个数，则按照nBytesNum读取，否则按照实际发送过来的字节个数读取
// 返回实际读取到的字节个数，pbtData由调用者分配并负责释放
KSJCOM_API  KSJCOM_ReadData( int nChannel, BYTE *pbtData, int nBytesNum );

// 关闭相应的COM口
KSJCOM_API  KSJCOM_Close( int nChannel );

// 在指定的COM口上添加一个解码器，并指定此解码器的地址及使用的协议
// 注意，同一个COM口上不要添加两个地址相同的解码器。
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

// 舵机控制
// nIndex 表示控制哪个舵机，nPosition 表示舵机的值
KSJCOM_API  KSJCOM_DuojiControl( int nChannel, int nIndex, int nPosition );


#ifdef __cplusplus
}
#endif

#endif

