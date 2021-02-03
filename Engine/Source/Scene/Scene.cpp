#include "stdafx.h"
#include "../Math/Color.h"
#include "../2D/Canvas.h"
#include "Actor.h"
#include "Scene.h"

namespace Led
{
	void Scene::_AddCamera(class Camera* cam)
	{
		set<Camera*>::const_iterator iter = _cameras.find(cam);
		if(iter == _cameras.end())
		{
			_cameras.insert(cam);
			if(!_camera)
				_camera = cam;
		}
	}
	void Scene::_RemoveCamera(Camera* cam)
	{
		set<Camera*>::const_iterator iter = _cameras.find(cam);
		if(iter != _cameras.end())
			_cameras.erase(iter);
	}
	Scene::Scene()
	{
		_camera = nullptr;
	}
	Scene::~Scene()
	{
		for(Actor* a: _actors)
			delete a;
	}

	bool Scene::IsActor(Actor *actor)
	{
		set<Actor*>::const_iterator iter = _actors.find(actor);
		if(iter != _actors.end())
			return true;
		return false;
	}

	bool Scene::IsCanvas(Canvas *canvas)
	{
		auto iter = _canvas.find(canvas);
		if(iter != _canvas.end())
			return true;
		return false;
	}

	bool Scene::AddCanvas(Canvas *canvas)
	{
		if(!IsCanvas(canvas))
		{
			_canvas.insert(canvas);
			return true;
		}
		return false;
	}

	bool Scene::AddActor(Actor *actor)
	{
		if(!IsActor(actor) && actor->BeforeAdd(this))
		{
			actor->Start();
			_actors.insert(actor);
			actor->AfterAdd(this);
			return true;
		}
		return false;
	}
	bool Scene::RemoveActor(Actor *actor)
	{
		if(actor->BeforeRemove(this))
		{
			set<Actor*>::const_iterator iter = _actors.find(actor);
			if(iter != _actors.end())
				_actors.erase(iter);
			delete actor;
			return true;
		}
		return false;
	}
	void Scene::Clear()
	{

	}
	void Scene::Update()
	{
		for(Actor* a: _actors)
			if(!a->GetParent())
				a->UpdateTransform();
	}

	void Scene::BeforeRender()
	{
		for(Actor* actor: _actors)
		{
			actor->Update();
			actor->BeforeRender();
		}
		for(Canvas* canvas: _canvas)
			canvas->BeforeRender();
	}

	void Scene::Render()
	{
		for(Actor* actor: _actors)
			actor->Render(this);
		for(Canvas* canvas: _canvas)
			canvas->Render(this);
	}
}