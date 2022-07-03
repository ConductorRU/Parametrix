#include "stdafx.h"
#include "Math/Header.h"
#include "Render/Header.h"
#include "Scene/Header.h"
namespace Led
{
	Render *Render::_this = nullptr;
	Render::Render()
	{
		_this = this;
	}
	void Render::RenderScene(Scene *scene)
	{
		scene->BeforeRender();

		Begin(Color(0.1f, 0.8f, 0.8f), 1.0f);
		scene->Render();
		End();
	}
}