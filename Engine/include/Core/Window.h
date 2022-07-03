#pragma once
namespace Led
{
	class Window
	{
	private:
		WNDCLASSEX _wClass;
		HWND _hWindow;
		HDC _hDC;
		bool _isFullscreen;
		LRESULT (*_func)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static map<HWND, Window*> _map;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		HWND GetHWND() {return _hWindow;};
		bool IsFullscreen() { return _isFullscreen; };
		Window();
		~Window();
		void Initialize(const wstring &name, uint width, uint height, bool fullscreen);
		void SetFunc(LRESULT (*func)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam));
		bool Update();
		void Clear();
	};
}