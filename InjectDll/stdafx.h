// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <sstream>
#include <assert.h>

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#define PSAPI_VERSION 1
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
#include <Pathcch.h>
#pragma comment(lib,"Pathcch.lib")
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_ttf.h"
#pragma comment(lib,"SDL/lib/x86/SDL2.lib")
#pragma comment(lib,"SDL/lib/x86/SDL2main.lib")
#pragma comment(lib,"SDL/lib/x86/SDL2_ttf.lib")