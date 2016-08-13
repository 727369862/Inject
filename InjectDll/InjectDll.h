// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� INJECTDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// INJECTDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef INJECTDLL_EXPORTS
#define INJECTDLL_API __declspec(dllexport)
#else
#define INJECTDLL_API __declspec(dllimport)
#endif
#include "HookApi.h"

INJECTDLL_API void GetProcessPath(DWORD dwProcessID, void *buffer);
INJECTDLL_API HRESULT GetDllPath(LPTSTR lpstrOut, LPTSTR lpstrExecPath, LPTSTR lpstrDllName);
INJECTDLL_API DWORD DisplayErrorText();
INJECTDLL_API char* WcharToChar(const wchar_t* wp);
INJECTDLL_API wchar_t* CharToWchar(const char* c);
INJECTDLL_API HRESULT Panic(LPSTR message);
INJECTDLL_API HRESULT Prompt();

INJECTDLL_API extern TCHAR g_DllName[];
INJECTDLL_API extern TCHAR g_ExecPath[];
INJECTDLL_API extern TCHAR g_strWndClass[];
INJECTDLL_API extern TCHAR g_strWndName[];
INJECTDLL_API extern TCHAR g_strNamedPipe[];
INJECTDLL_API extern BOOL g_bInjected;
INJECTDLL_API extern HWND g_hWnd;
INJECTDLL_API extern HWND g_hWndTarget;
INJECTDLL_API extern HANDLE g_hProcess;
INJECTDLL_API extern DWORD g_dwBufSize;

void Attach(void);
void Detach(void);