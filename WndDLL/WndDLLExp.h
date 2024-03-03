#pragma once
#include <Windows.h>

extern "C" __declspec(dllexport) HRESULT __stdcall LoadWnd(DWORD);
extern "C" __declspec(dllexport) HRESULT __stdcall CloseWnd(DWORD);
