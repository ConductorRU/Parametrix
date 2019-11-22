#include "stdafx.h"
#include "../Math/Transform.h"
#include "Window.h"
#include "../Render/Render.h"
#include "Engine.h"
#include "Input.h"

namespace Led
{
	Input::Input()
	{
		_frame = 0;
		_updFrame = 0;
		z_whell = 0;
		//Log::Get()->Debug("InputMgr init");
		memset(z_pressed, 0, sizeof(z_pressed));
		memset(z_mpressed, 0, sizeof(z_mpressed));
		memset(z_hit, 0, sizeof(z_hit));
		memset(z_mhit, 0, sizeof(z_mhit));
		memset(z_released, 0, sizeof(z_released));
		memset(z_mreleased, 0, sizeof(z_mreleased));
		_isRelative = false;
	}

	void Input::SetMode(bool isRelative)
	{
		_isRelative = isRelative;
		Engine::Get()->ShowCursor(!isRelative);

	}
	void Input::Close()
	{
		
	}
	bool Input::IsKeyPressed(KEY_TYPE code)
	{
		return z_pressed[code];
	}
	bool Input::IsMousePressed(uint code)
	{
		return z_mpressed[code];
	}
	bool Input::IsMouseHit(uint code)
	{
		return z_mhit[code];
	}
	bool Input::IsKeyHit(KEY_TYPE code)
	{
		return z_hit[code];
	}
	bool Input::IsMouseReleased(uint code)
	{
		return z_mreleased[code];
	}
	bool Input::IsKeyReleased(KEY_TYPE code)
	{
		return z_released[code];
	}
	Point2 Input::GetCursorPos()
	{
		return z_cursor;
	}
	Point2 Input::GetCursorRel()
	{
		return _relative;
	}
	string Input::GetInputText(KEY_TYPE code)
	{
		switch(code)
		{
		case KEY_NUMPAD1: return "1";
		case KEY_NUMPAD2: return "2";
		case KEY_NUMPAD3: return "3";
		case KEY_NUMPAD4: return "4";
		case KEY_NUMPAD5: return "5";
		case KEY_NUMPAD6: return "6";
		case KEY_NUMPAD7: return "7";
		case KEY_NUMPAD8: return "8";
		case KEY_NUMPAD9: return "9";
		case KEY_NUMPAD0: return "0";
		case KEY_SPACE:		return " ";
		case KEY_TAB:	  	return "	";
		}
		if(IsKeyPressed(KEY_SHIFT))
		{
			switch(code)
			{
			case KEY_1: return "!";
			case KEY_2: return "@";
			case KEY_3: return "#";
			case KEY_4: return "$";
			case KEY_5: return "%";
			case KEY_6: return "^";
			case KEY_7: return "&";
			case KEY_8: return "*";
			case KEY_9: return "(";
			case KEY_0: return ")";
			case KEY_MINUS: return "_";
			case KEY_PLUS:  return "+";
			case KEY_Q: return "Q";
			case KEY_W: return "W";
			case KEY_E: return "E";
			case KEY_R: return "R";
			case KEY_T: return "T";
			case KEY_Y: return "Y";
			case KEY_U: return "U";
			case KEY_I: return "I";
			case KEY_O: return "O";
			case KEY_P: return "P";
			case KEY_LBRACKET: return "{";
			case KEY_RBRACKET: return "}";
			case KEY_A: return "A";
			case KEY_S: return "S";
			case KEY_D: return "D";
			case KEY_F: return "F";
			case KEY_G: return "G";
			case KEY_H: return "H";
			case KEY_J: return "J";
			case KEY_K: return "K";
			case KEY_L: return "L";
			case KEY_SEMICOLON: return ":";
			case KEY_APOSTROPHE: return "\"";
			case KEY_Z: return "Z";
			case KEY_X: return "X";
			case KEY_C: return "C";
			case KEY_V: return "V";
			case KEY_B: return "B";
			case KEY_N: return "N";
			case KEY_M: return "M";
			case KEY_COMMA: return "<";
			case KEY_PERIOD: return ">";
			case KEY_SLASH: return "?";
			case KEY_GRAVE: return "~";
			case KEY_BACKSLASH: return "|";
			}
		}
		else switch(code)
		{
		case KEY_1: return "1";
		case KEY_2: return "2";
		case KEY_3: return "3";
		case KEY_4: return "4";
		case KEY_5: return "5";
		case KEY_6: return "6";
		case KEY_7: return "7";
		case KEY_8: return "8";
		case KEY_9: return "9";
		case KEY_0: return "0";
		case KEY_MINUS: return "-";
		case KEY_PLUS:  return "=";
		case KEY_Q: return "q";
		case KEY_W: return "w";
		case KEY_E: return "e";
		case KEY_R: return "r";
		case KEY_T: return "t";
		case KEY_Y: return "y";
		case KEY_U: return "u";
		case KEY_I: return "i";
		case KEY_O: return "o";
		case KEY_P: return "p";
		case KEY_LBRACKET: return "[";
		case KEY_RBRACKET: return "]";
		case KEY_A: return "a";
		case KEY_S: return "s";
		case KEY_D: return "d";
		case KEY_F: return "f";
		case KEY_G: return "g";
		case KEY_H: return "h";
		case KEY_J: return "j";
		case KEY_K: return "k";
		case KEY_L: return "l";
		case KEY_SEMICOLON: return ";";
		case KEY_APOSTROPHE: return "'";
		case KEY_Z: return "z";
		case KEY_X: return "x";
		case KEY_C: return "c";
		case KEY_V: return "v";
		case KEY_B: return "b";
		case KEY_N: return "n";
		case KEY_M: return "m";
		case KEY_COMMA: return ",";
		case KEY_PERIOD: return ".";
		case KEY_SLASH: return "/";
		case KEY_GRAVE: return "`";
		case KEY_BACKSLASH: return "\\";
		}
		return "";
	}
	void Input::ClipToWindow()
  {
		HWND mWindow = Engine::Get()->GetWindow()->GetHWND();
    assert(mWindow != nullptr);

    RECT rect;
    GetClientRect(mWindow, &rect);

    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;

    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;

    MapWindowPoints(mWindow, nullptr, &ul, 1);
    MapWindowPoints(mWindow, nullptr, &lr, 1);

    rect.left = ul.x;
    rect.top = ul.y;

    rect.right = lr.x;
    rect.bottom = lr.y;

    ClipCursor(&rect);
  }
	void Input::SetWinRect(const RECT &winrect)
	{
		z_rect.left = winrect.left;
		z_rect.right = winrect.right;
		z_rect.top = winrect.top;
		z_rect.bottom = winrect.bottom;
	}

	void Input::Run(const UINT &msg, WPARAM wParam, LPARAM lParam)
	{
		if(_frame != _updFrame)
		{
			_updFrame = _frame;
			memset(z_released, 0, sizeof(z_released));

		}
		KEY_TYPE KeyIndex;
		wchar_t buffer[1];
		BYTE lpKeyState[256];
		switch(msg)
		{
		case WM_INPUT:

				
		break;
		case WM_KEYDOWN:
			KeyIndex = static_cast<KEY_TYPE>(wParam);
			if(!z_pressed[KeyIndex])
				z_hit[KeyIndex] = true;
			else
				z_hit[KeyIndex] = false;
			z_pressed[KeyIndex] = true;
			GetKeyboardState(lpKeyState);
			ToUnicode((uint)wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
			break;
		case WM_KEYUP:
			KeyIndex = static_cast<KEY_TYPE>(wParam);
			z_released[KeyIndex] = true;
			z_pressed[KeyIndex] = false;
			GetKeyboardState(lpKeyState);
			ToUnicode((uint)wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
			break;
		case WM_LBUTTONDOWN:
			z_mpressed[0] = true;
			break;
		case WM_LBUTTONUP:
			z_mpressed[0] = false;
			break;
		case WM_RBUTTONDOWN:
			z_mpressed[2] = true;
			break;
		case WM_RBUTTONUP:
			z_mpressed[2] = false;
			break;
		case WM_MBUTTONDOWN:
			z_mpressed[1] = true;
			break;
		case WM_MBUTTONUP:
			z_mpressed[1] = false;
			break;
		case WM_MOUSEMOVE:

			break;
		case WM_MOUSEWHEEL:
			z_whell = (short)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			break;
		}
	}
	void Input::Update()
	{
		POINT Position;
		if(::GetCursorPos(&Position))
		{
			Position.x -= z_rect.left;
			Position.y -= z_rect.top;
			z_cursor.x = Position.x;
			z_cursor.y = Position.y;
			if(_frame)
			{
				Point2 difference = Point2(z_cursor.x - _lastCursor.x, z_cursor.y - _lastCursor.y);
				_relative = difference;
				if(_isRelative)
				{

					SetCursorPos(Engine::Get()->GetRender()->GetWidth()/2, Engine::Get()->GetRender()->GetHeight()/2);
					if(::GetCursorPos(&Position))
					{
						Position.x -= z_rect.left;
						Position.y -= z_rect.top;
						_lastCursor.x = Position.x;
						_lastCursor.y = Position.y;
					}
				}
			}
			else
				_lastCursor = z_cursor;
		}
		++_frame;
	}
}