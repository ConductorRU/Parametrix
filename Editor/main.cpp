#include "Parametrix.h"
#include "Game.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	_CrtMemState ms = {};
	_CrtMemCheckpoint(&ms);
	{
		//FreeType freeType;
		//FreeTypeFont *font = freeType.LoadFont("fonts/arial.ttf");
		Game game;
		game.Update();
	}
	_CrtMemDumpAllObjectsSince(&ms);
	return 0;
}
