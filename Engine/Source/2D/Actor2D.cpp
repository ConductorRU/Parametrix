#include "stdafx.h"
#include "Actor2D.h"

namespace Led
{
	Actor2D::Actor2D()
	{

	}

	Actor2D::~Actor2D()
	{

	}

	void Actor2D::AddChild(Actor2D *node)
	{
		node->_parent = this;
		_children.push_back(node);
	}

	void Actor2D::RemoveChild(Actor2D *actor)
	{
		auto iter = _children.begin();
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

	void Actor2D::UpdateTransform()
	{
		if(_parent)
			_worldTransform = _parent->_worldTransform*_localTransform;
		else
			_worldTransform = _localTransform;
		for(Actor2D* a: _children)
			a->UpdateTransform();
	}
	Transform2D &Actor2D::GetLocalTransform()
	{
		return _localTransform;
	}
	Transform2D &Actor2D::GetWorldTransform()
	{
		return _worldTransform;
	}

	void Actor2D::SetSize(float sizeX, float sizeY)
	{
		_size.x = sizeX;
		_size.y = sizeY;
	}
}