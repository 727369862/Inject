// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <memory>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <windows.h>
#include "../InjectDll/InjectDll.h"
#ifdef _DEBUG
#pragma comment(lib,"../Debug/InjectDll.lib")
#else
#pragma comment(lib,"../Release/InjectDll.lib")
#endif