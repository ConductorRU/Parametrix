#include "Source/Parametrix.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	_CrtMemState ms;
	_CrtMemCheckpoint(&ms);
	{
		Game game;
		game.Update();
	}
	_CrtMemDumpAllObjectsSince(&ms);
	return 0;
}
