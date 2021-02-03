#pragma once
#include "../Math/Transform.h"

namespace Led
{
	class ActorBase
	{
	private:
		wstring _name;
	public:
		virtual void BeforeRender() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void Render(class Scene* scene) {};
		virtual bool BeforeAdd(Scene* scene) { return true; };
		virtual void AfterAdd(Scene* scene) {};
		virtual bool BeforeRemove(Scene* scene) { return true; };
	};
	class Actor: public ActorBase
	{
	private:
		Transform _worldTransform;
		Transform _localTransform;
		Actor *_parent;
		vector<Actor*> _children;
		
	public:
		Actor* GetParent() { return _parent; };
		Actor();
		virtual ~Actor();
		void UpdateTransform();
		Transform &GetLocalTransform();
		Transform &GetWorldTransform();
		void AddChild(Actor *actor);
		void RemoveChild(Actor *actor);
	};
	class Actor2d: public ActorBase
	{
	private:
		Transform _worldTransform;
		Transform _localTransform;
		Actor2d *_parent;
		vector<Actor2d*> _children;
	public:
		Actor2d* GetParent() { return _parent; };
		Actor2d();
		virtual ~Actor2d();
		void UpdateTransform();
		Transform &GetLocalTransform();
		Transform &GetWorldTransform();
		void AddChild(Actor2d *actor);
		void RemoveChild(Actor2d *actor);
	};
}