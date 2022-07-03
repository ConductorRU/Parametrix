#include "stdafx.h"
#include "Core/Window.h"

namespace Led
{
	map<HWND, Window*> Window::_map = {};
	Window::Window()
	{
		_hDC = nullptr;
		_func = nullptr;
		_hWindow = nullptr;
		_wClass = {};
		_isFullscreen = false;
	}
	Window::~Window()
	{
		Clear();
	}
	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto f = _map.find(hWnd);
		if(f == _map.end())
			return DefWindowProc(hWnd, msg, wParam, lParam);
		if(f->second->_func)
			return f->second->_func(hWnd, msg, wParam, lParam);
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	void Window::Initialize(const wstring &name, uint width, uint height, bool fullscreen)
	{
		int sx = GetSystemMetrics(SM_CXSCREEN);
		int sy = GetSystemMetrics(SM_CYSCREEN);
		if((int)width > sx - 20)
			width = sx - 20;
		if((int)height > sy - 100)
			height = sy - 100;
		_wClass.cbSize = sizeof(WNDCLASSEX);
		_wClass.style = CS_CLASSDC;
		_wClass.lpfnWndProc = &Window::WndProc;
		_wClass.cbClsExtra = 0L;
		_wClass.cbWndExtra = 0L;
		_wClass.hInstance = GetModuleHandle(NULL);
		_wClass.hIcon = LoadIcon(_wClass.hInstance, MAKEINTRESOURCE(1));
		_wClass.hCursor = NULL;
		_wClass.hbrBackground = NULL;
		_wClass.lpszMenuName = NULL;
		_wClass.lpszClassName = L"Led";
		_wClass.hIconSm = LoadIcon(_wClass.hInstance, MAKEINTRESOURCE(1));
		RegisterClassEx(&_wClass);
		DWORD style = 0;
		if(!fullscreen)
			style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX;
		_hWindow = CreateWindowW(L"Led", name.c_str(), style, 0, 0, width, height, NULL, NULL, _wClass.hInstance, NULL);
		if(!_hWindow)
      MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		_map[_hWindow] = this;
		DWORD le = GetLastError();
		_hDC = GetDC(_hWindow);
		WINDOWINFO info;
		GetWindowInfo(_hWindow, &info);
		SetWindowPos(_hWindow, NULL, 0, 0, info.rcWindow.right + info.rcClient.left + (info.rcWindow.right - info.rcClient.right), info.rcWindow.bottom + info.rcClient.top + (info.rcWindow.bottom - info.rcClient.bottom), SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		ShowWindow(_hWindow, SW_SHOWNORMAL);
		_isFullscreen = fullscreen;
	}
	void Window::SetFunc(LRESULT (*func)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam))
	{
		_func = func;
	}
	bool Window::Update()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while(PeekMessage(&msg, _hWindow, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}
	void Window::Clear()
	{
		DestroyWindow(_hWindow);
		UnregisterClass(L"Led", _wClass.hInstance);
		_map.erase(_hWindow);
	}
}