#include "stdafx.h"
#include "Render/Header.h"
#include "Scene/Header.h"
#include "Header.h"
namespace Led
{
	Engine *Engine::_this = nullptr;
	DEBUG_TYPE Debug::_type = DEBUG_NONE;
	void Time::Update()
	{
		chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
		if(_frames != 0)
		{
			_mcSec = chrono::duration_cast<chrono::microseconds>(end - _last).count();
			spf = _mcSec/1000000.0f;
		}
		_Sec += _mcSec;
		if(_Sec > 1000000)
		{
			secCount += (int)(_Sec/1000000);
			_Sec = _Sec%1000000;
			fps = _framesPerSec;
			_framesPerSec = 0;
		}
		_last = end;
		++_frames;
		++_framesPerSec;
	}
	Engine::Engine()
	{
		_this = this;
		_window = nullptr;
		_cursor = nullptr;
		_cursorType = CURSOR_NONE;
		_exit = false;
		_time = {};
		_manager = new Manager();
		_scene = new Scene();
		_input = new Input();
	}
	Engine::~Engine()
	{
		Clear();
		delete _input;
		delete _scene;
		delete _manager;
	}
	void Engine::ShowCursor(bool enable)
	{
		::ShowCursor(enable);
	}
	void Engine::Initialize(uint width, uint height, bool isFullscreen, const wstring &title)
	{
		_window = new Window;
		_window->Initialize(title, width, height, isFullscreen);
		_window->SetFunc(Engine::WindProc);
		_render = new RenderDX12;
		_render->Initialize(_window, width, height);
		SetCursor(CURSOR_ARROW);
	}
	LRESULT Engine::WindProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_CREATE:
			return 0;
		case WM_MOVE:
			return 0;
		case WM_QUIT:
			return 0;
		case WM_CLOSE:
			Engine::Get()->_exit = true;
			return 0;
		case WM_SIZE:
			//Engine::Get()->GetRender()->Resize(LOWORD(lParam), HIWORD(lParam));
			return 0;
			//case WM_SIZING:
		case WM_MOUSEMOVE: case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_MOUSEWHEEL: case WM_KEYDOWN: case WM_KEYUP:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		if(Engine::Get()->GetInput())
			Engine::Get()->GetInput()->Run(message, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	void Engine::SetCursor(CURSOR_TYPE id)
	{
		if(_cursorType == id)
			return;
		_cursorType = id;
		switch(id)
		{
		case CURSOR_ARROW: _cursor = LoadCursor(NULL, IDC_ARROW); break;
		case CURSOR_TEXT: _cursor = LoadCursor(NULL, IDC_IBEAM); break;
		case CURSOR_WAIT: _cursor = LoadCursor(NULL, IDC_WAIT); break;
		case CURSOR_CROSS: _cursor = LoadCursor(NULL, IDC_CROSS); break;
		case CURSOR_UPARROW: _cursor = LoadCursor(NULL, IDC_UPARROW); break;
		case CURSOR_DIAGONAL1: _cursor = LoadCursor(NULL, IDC_SIZENWSE); break;
		case CURSOR_DIAGONAL2: _cursor = LoadCursor(NULL, IDC_SIZENESW); break;
		case CURSOR_HORIZONTAL: _cursor = LoadCursor(NULL, IDC_SIZEWE); break;
		case CURSOR_VERTICAL: _cursor = LoadCursor(NULL, IDC_SIZENS); break;
		case CURSOR_SIZE: _cursor = LoadCursor(NULL, IDC_SIZEALL); break;
		case CURSOR_POINTER: _cursor = LoadCursor(NULL, IDC_HAND); break;
		}
		::SetCursor(_cursor);
	}
	void Engine::Render()
	{
		_scene->Update();
		_render->RenderScene(_scene);
	}
	bool Engine::Update()
	{
		if(_exit)
			return false;
		_time.Update();
		_input->Update();
		return _window->Update();
	}
	void Engine::Clear()
	{
		if(_render)
			delete _render;
		if(_window)
			delete _window;
		_render = nullptr;
		_window = nullptr;
	}
	void Engine::Exit()
	{
		_exit = true;
	}
}