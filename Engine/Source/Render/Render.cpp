#include "stdafx.h"
#include "../Math/Header.h"
#include "Header.h"
#include "../Scene/Header.h"
namespace Led
{
	Render *Render::_this = nullptr;
	Render::Render()
	{
		_this = this;
	}
	void Render::RenderScene(Scene *scene)
	{
		for(Actor* actor: scene->_actors)
			actor->BeforeRender();

		Begin(Color(0.1f, 0.8f, 0.8f), 1.0f);
		for(Actor* actor: scene->_actors)
			actor->Render(scene);
		End();
	}
}