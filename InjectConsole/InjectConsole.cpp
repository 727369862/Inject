// InjectConsole.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <thread>
#include "Pipe.h"

int main()
{
    HRESULT hr;

    printf("�����ʼ����...\n");
    if ((hr = Prompt()) != S_OK)
    {
        return hr;
    }
    printf("��Ȩ�ɹ�!\n");

    //��ִ���ļ�·��
    TCHAR lpstrPath[MAX_PATH];
    GetProcessPath(GetCurrentProcessId(), lpstrPath);
    printf("����·��: %S\n", lpstrPath);
    //ע��dll·��
    TCHAR szLibPath[MAX_PATH];
    if (GetDllPath(szLibPath, lpstrPath, g_DllName) != S_OK)
    {
        printf("�Ҳ���·��\n");
        system("pause");
    }
    printf("ע��·��: %S\n", szLibPath);
    //ȡ��DLLģ����
    //ȡ��Kernel32.dll�ĺ�����ַ
    auto hKernel32 = ::GetModuleHandleA("Kernel32");
    printf("(HMODULE) Kernel32.dll:     0x%p\n", hKernel32);
    //ȡ��fpcLoadLibrary�ĺ�����ַ
    auto fpcLoadLibrary = ::GetProcAddress(hKernel32, "LoadLibraryA");
    printf("(FARPROC) LoadLibrary:      0x%p\n", fpcLoadLibrary);
    //ȡ��fpcFreeLibrary�ĺ�����ַ
    auto fpcFreeLibrary = ::GetProcAddress(hKernel32, "FreeLibrary");
    printf("(FARPROC) FreeLibrary:      0x%p\n", fpcFreeLibrary);
    //ȡ��fpcGetConsoleWindow�ĺ�����ַ
    auto fpcGetConsoleWindow = GetProcAddress(hKernel32, "GetConsoleWindow");
    printf("(FARPROC) GetConsoleWindow: 0x%p\n", fpcGetConsoleWindow);
    typedef HWND(FAR WINAPI *pGetConsoleWindow)();
    //����̨���ھ��
    auto hwConsoleWindow = pGetConsoleWindow(fpcGetConsoleWindow)();
    char lpstrConsoleTitle[MAX_PATH];

    ::SetConsoleTitleA("Inject Test");
    ::GetConsoleTitleA(lpstrConsoleTitle, MAX_PATH);

    printf("���ھ��: 0x%p\n", hwConsoleWindow);
    printf("���ڱ���: %s\n", lpstrConsoleTitle);
    printf("���̾��: 0x%08X\n", GetCurrentProcessId());
    printf("�߳̾��: 0x%08X\n", GetCurrentThreadId());
    auto hStd_OP = ::GetStdHandle(STD_OUTPUT_HANDLE);
    printf("��׼���: 0x%p\n", hStd_OP);

    if (int(::ShellExecute(nullptr, _T("open"), g_ExecPath, nullptr, nullptr, SW_SHOWNORMAL)) > 32)
    {
        printf("�����Ѵ�!\n");
    }
    else
    {
        return Panic("ShellExecuteA");
    }

    auto hWnd = ::FindWindow(g_strWndClass, g_strWndName);
    while (!hWnd)
    {
        hWnd = ::FindWindow(g_strWndClass, g_strWndName);
        ::Sleep(1000);
    }

    printf("���ھ��: 0x%p\n", hWnd);

    HANDLE hRemoteProcess;
    DWORD dwPId;
    DWORD dwTId;
    DWORD dwPathLength;
    HANDLE hThread;
    DWORD hLibModule;
    LPVOID pLibRemoteSrc;

    dwTId = ::GetWindowThreadProcessId(hWnd, &dwPId);
    printf("���̾��: 0x%08X\n", dwPId);
    printf("�߳̾��: 0x%08X\n", dwTId);
    //��ȡԶ�̽��̵�HANDLE���
    hRemoteProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPId);
    while (!hRemoteProcess)
    {
        hRemoteProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPId);
        ::Sleep(1000);
    }
    printf("�򿪽���: 0x%p\n", hRemoteProcess);

    //ע��DLL·��
    auto szLibPathA = std::make_unique<char*>(WcharToChar(szLibPath));
    auto nLibPathLength = sizeof(char)*(strlen(*szLibPathA)) + 1;

    //��Զ�̽�����ΪDLL���Ʒ����ڴ�ռ�
    if (!((pLibRemoteSrc = ::VirtualAllocEx(hRemoteProcess, nullptr, nLibPathLength, MEM_COMMIT, PAGE_READWRITE))))
    {
        return Panic("VirtualAllocEx");
    }
    else
    {
        printf("ִ�е�ַ: 0x%p\n", pLibRemoteSrc);
    }
    //д��DLL����(��������·��)���ѷ���Ĵ洢�ռ���
    if (!::WriteProcessMemory(hRemoteProcess, pLibRemoteSrc, LPVOID(*szLibPathA), nLibPathLength, &dwPathLength))
    {
        return Panic("WriteProcessMemory");
    }
    if (!::ReadProcessMemory(hRemoteProcess, pLibRemoteSrc, LPVOID(*szLibPathA), nLibPathLength, &dwPathLength))
    {
        return Panic("ReadProcessMemory");
    }
    printf("ע��·��: %s\n", *szLibPathA);
    printf("·������: %d\n", dwPathLength);
    //ӳ��DLL��Զ�̽�����
    hThread = ::CreateRemoteThread(hRemoteProcess, nullptr, NULL,
        LPTHREAD_START_ROUTINE(fpcLoadLibrary), pLibRemoteSrc, NULL, nullptr);
    if (!hThread)
    {
        return Panic("CreateRemoteThread");
    }
    printf("�߳̾��: 0x%p\n", hThread);
    //�ܵ���ʼ��
    ReadingFromClient();
    //�ȴ�Զ���߳���ֹ
    ::WaitForSingleObject(hThread, INFINITE);
    //����Զ���̵߳��˳�����
    ::GetExitCodeThread(hThread, &hLibModule);
    printf("ģ����: 0x%08X\n", hLibModule);
    //���Thread
    ::CloseHandle(hThread);
    ::VirtualFreeEx(hRemoteProcess, pLibRemoteSrc, MAX_PATH, MEM_RELEASE);
    hThread = nullptr;

    system("pause");

    //����
    printf("�رմ���...\n");
    //��Ŀ������н�DLLж��
    hThread = ::CreateRemoteThread(hRemoteProcess, nullptr, NULL,
        LPTHREAD_START_ROUTINE(fpcFreeLibrary), LPVOID(hLibModule), NULL, nullptr);
    printf("ж���߳�: 0x%p\n", hThread);
    //�ȴ���Ӧ
    ::WaitForSingleObject(hThread, INFINITE);
    //�ͷ�VirtualAllocEx������ڴ�
    printf("�ͷ�Զ��Dll...\n");
    ::VirtualFreeEx(hRemoteProcess, pLibRemoteSrc, MAX_PATH, MEM_RELEASE);
    ::CloseHandle(hThread);
    hThread = nullptr;
    printf("�رմ��ھ��...\n");
    CloseHandle(hRemoteProcess);
    hRemoteProcess = nullptr;
    SendMessage(hWnd, WM_CLOSE, NULL, NULL);
    hWnd = nullptr;

    printf("�����ѹر�\n");
    return 0;
}
