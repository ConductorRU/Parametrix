#pragma once
#include "../Scene/Actor.h"
#include "../Math/Transform2D.h"
namespace Led
{
	class Actor2D: public ActorBase
	{
	protected:
		float2 _size;
		Transform2D _worldTransform;
		Transform2D _localTransform;
		Actor2D *_parent;
		vector<Actor2D*> _children;
	public:
		Actor2D* GetParent() { return _parent; };
		Actor2D();
		virtual ~Actor2D();
		void UpdateTransform();
		Transform2D &GetLocalTransform();
		Transform2D &GetWorldTransform();
		void AddChild(Actor2D *actor);
		void RemoveChild(Actor2D *actor);
		void SetSize(float sizeX, float sizeY);
	};
}
