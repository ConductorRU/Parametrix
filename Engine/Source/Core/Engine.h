#pragma once
namespace Led
{
	enum CURSOR_TYPE
	{
		CURSOR_NONE = 0,
		CURSOR_ARROW = 1,
		CURSOR_TEXT = 2,
		CURSOR_WAIT = 3,
		CURSOR_CROSS = 4,
		CURSOR_UPARROW = 5,
		CURSOR_DIAGONAL1 = 6,
		CURSOR_DIAGONAL2 = 7,
		CURSOR_HORIZONTAL = 8,
		CURSOR_VERTICAL = 9,
		CURSOR_SIZE = 10,
		CURSOR_POINTER = 11,
	};
	struct Time
	{
	private:
		long long _frames;
		chrono::time_point<chrono::system_clock> _last;
		long long _mcSec;
		long long _Sec;
		uint _framesPerSec;
	public:
		uint	secCount;
		float spf;
		uint fps;
		void Update();
	};
	class Engine
	{
	private:
		static Engine* _this;
		class Window* _window;
		class Render* _render;
		class Scene* _scene;
		class Input* _input;
		class Manager* _manager;
		Time _time;
		HCURSOR _cursor;
		CURSOR_TYPE _cursorType;
		bool _exit;
	public:
		static Engine *Get() { return _this; };
		Time *GetTime() { return &_time; };
		Manager *GetManager() { return _manager; };
		Scene *GetScene() { return _scene; };
		Input* GetInput() { return _input; }
		Window* GetWindow() { return _window; }
		Render* GetRender() { return _render; }
		Engine();
		~Engine();
		void ShowCursor(bool enable);
		void Initialize(uint width = 1024, uint height = 768, bool isFullscreen = false, const wstring &title = L"LedEngine");
		static LRESULT WindProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
		void SetCursor(CURSOR_TYPE id);
		void Render();
		bool Update();
		void Clear();
		void Exit();
	};
}