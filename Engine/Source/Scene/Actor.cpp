#include "stdafx.h"
#include "Scene/Actor.h"

namespace Led
{
	Actor::Actor()
	{
		_parent = nullptr;
	}
	Actor::~Actor()
	{
	
	}
	void Actor::UpdateTransform()
	{
		if(_parent)
			_worldTransform = _parent->_worldTransform*_localTransform;
		else
			_worldTransform = _localTransform;
		for(Actor* a: _children)
			a->UpdateTransform();
	}
	Transform &Actor::GetLocalTransform()
	{
		return _localTransform;
	}
	Transform &Actor::GetWorldTransform()
	{
		return _worldTransform;
	}
	void Actor::AddChild(Actor *actor)
	{
		actor->_parent = this;
		_children.push_back(actor);
	}
	void Actor::RemoveChild(Actor *actor)
	{
		vector<Actor*>::const_iterator iter = _children.begin();
		while(iter != _children.end())
		{
			if(*iter == actor)
			{
				_children.erase(iter);
				break;
			}
			++iter;
		}
	}

	Actor2d::Actor2d()
	{
		_parent = nullptr;
	}
	Actor2d::~Actor2d()
	{
	
	}
	void Actor2d::UpdateTransform()
	{
		if(_parent)
			_worldTransform = _parent->_worldTransform*_localTransform;
		else
			_worldTransform = _localTransform;
		for(Actor2d* a: _children)
			a->UpdateTransform();
	}
	Transform &Actor2d::GetLocalTransform()
	{
		return _localTransform;
	}
	Transform &Actor2d::GetWorldTransform()
	{
		return _worldTransform;
	}
	void Actor2d::AddChild(Actor2d *actor)
	{
		actor->_parent = this;
		_children.push_back(actor);
	}
	void Actor2d::RemoveChild(Actor2d *actor)
	{
		vector<Actor2d*>::const_iterator iter = _children.begin();
		while(iter != _children.end())
		{
			if(*iter == actor)
			{
				_children.erase(iter);
				break;
			}
			++iter;
		}
	}
}