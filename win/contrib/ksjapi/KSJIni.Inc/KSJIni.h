#ifndef    __KSJ_INI_H__
#define    __KSJ_INI_H__

#ifdef _WIN64
#pragma message("_WIN64 defined")
#else
#pragma message("_WIN64 not defined")
#endif

#ifdef _UNICODE
#pragma message("_UNICODE defined")
#else
#pragma message("_UNICODE not defined")
#endif

#ifdef KSJINI_EXPORTS						    // ���붯̬���ӿ�ʱ����
    #pragma message("export dynamic ksjini lib")
	#define KSJINI_API __declspec(dllexport)  int WINAPI
#elif defined KSJINI_STATIC			            // ���뾲̬���ӿ�ʱ�������ʹ�þ�̬���ӿ��Ƕ���
    #pragma message("export or import static ksjini lib")  
    #define KSJINI_API int WINAPI
#else
    #pragma message("import dynamic ksjini lib")
	#define KSJINI_API __declspec(dllimport)  int WINAPI
#endif


#include <windows.h>

#define KSJINI_SUCCESS 0
#define KSJINI_FAIL    1

#ifdef __cplusplus
extern "C"{
#else
#endif

// strFullPathName������ini�ļ�������·����
// ��ʼ���������ᴴ��ini�ļ���ֻ�ж�ini�ļ�����д���ʱ��Ż��Զ�������
KSJINI_API KSJINI_Init(OUT HANDLE *hIni, IN LPCTSTR strFullPathName);

// ���ݵ����ߵ�hModule�������������ͬ���ļ����´���һ������ΪstrIniFileName���ļ���
// ��ʼ���������ᴴ��ini�ļ���ֻ�ж�ini�ļ�����д���ʱ��Ż��Զ�������
// hModule=NULL,��ʾ��ǰ�ĵ����ߡ���̬���ӿ���ں���DllMain�ĵ�һ������HANDLE hModule������Ϊ��̬���hModule
KSJINI_API KSJINI_InitEx(OUT HANDLE *hIni, IN HMODULE hModule, IN LPCTSTR strIniFileName);

KSJINI_API KSJINI_UnInit( IN  HANDLE *hIni );

KSJINI_API KSJINI_GetString( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR szKey, LPCTSTR szDefault, LPTSTR szValue );
KSJINI_API KSJINI_GetBOOL  ( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR szKey, BOOL bDefault,     BOOL *bValue );
KSJINI_API KSJINI_GetDWORD ( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR szKey, DWORD dwDefault,   DWORD *dwValue ); 


KSJINI_API KSJINI_SetString( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR  szKey, LPCTSTR strVal );
KSJINI_API KSJINI_SetInt   ( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR  szKey, int nVal       );
KSJINI_API KSJINI_SetBOOL  ( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR  szKey, BOOL bVal      );

#ifdef __cplusplus
}
#endif

#endif