#include "stdafx.h"
#include "Pipe.h"

HRESULT ReadingFromClient()
{
    printf("�����ܵ�!\n");

    //�����ܵ�
    auto hPipe = CreateNamedPipe(g_strNamedPipe, PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, g_dwBufSize, g_dwBufSize,
        NMPWAIT_USE_DEFAULT_WAIT, nullptr);

    if (INVALID_HANDLE_VALUE == hPipe)
    {
        hPipe = nullptr;
        return Panic("CreateNamedPipe");
    }

    g_bPipe = TRUE;
    printf("�ܵ����: 0x%p\n", hPipe);

    while (g_bPipe)
    {
        printf("[�ܵ�] �ȴ�����...\n");

        //�ȴ��ͻ���
        if (!ConnectNamedPipe(hPipe, nullptr))
        {
            CloseHandle(hPipe);
            return Panic("ConnectNamedPipe");
        }

        printf("[�ܵ�] �ܵ����ӳɹ�!\n");

        //��ȡ�ܵ��е�����
        DWORD nReadNum;
        auto hHeap = GetProcessHeap();
        auto szReadBuf = static_cast<char*>(HeapAlloc(hHeap, 0, (g_dwBufSize + 1)*sizeof(char)));
        while (ReadFile(hPipe, szReadBuf, g_dwBufSize, &nReadNum, nullptr))
        {
            if (nReadNum == 7 && strncmp(szReadBuf, "Detach!", 7) == 0)
                g_bPipe = FALSE;
            szReadBuf[nReadNum] = '\0';
            printf("[�ܵ�] >> %s\n", szReadBuf);
        }
        HeapFree(hHeap, 0, szReadBuf);
        printf("[�ܵ�] �ر�����\n");

        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
    }
    
    printf("[�ܵ�] �رչܵ�\n");
    CloseHandle(hPipe);

    return S_OK;
}