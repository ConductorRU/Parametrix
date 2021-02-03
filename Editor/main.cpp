#include "Source/Parametrix.h"
#include "Game.h"

struct A
{
	int x;
	REFLECT()
};
REFLECT_STRUCT_BEGIN(A)
REFLECT_STRUCT_MEMBER(x)
REFLECT_STRUCT_END()

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	_CrtMemState ms = {};
	A a;
	a.x = 5;
	auto aaa = a.Reflection.members;
	TypeDescriptor* typeDesc = TypeResolver<A>::get();
	//a.Reflection.
	_CrtMemCheckpoint(&ms);
	{
		Game game;
		game.Update();
	}
	_CrtMemDumpAllObjectsSince(&ms);
	return 0;
}
