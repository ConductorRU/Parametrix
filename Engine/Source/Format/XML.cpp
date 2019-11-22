#include "stdafx.h"
#include "XML.h"
namespace Led
{
	XMLNode::XMLNode()
	{
		_parent = nullptr;
	}
	XMLNode::~XMLNode()
	{
		for(XMLNode* child: _children)
			delete child;
	}
	void XMLNode::SetName(const wstring& name)
	{
		_name = name;
	}
	void XMLNode::AddChild(XMLNode *child)
	{
		_children.push_back(child);
	}
	void XMLNode::SetAttribute(const wstring& name, const wstring& value)
	{
		_attr[name] = value;
	}
	wstring XMLNode::GetName()
	{
		return _name;
	}
	XMLNode* XMLNode::GetParent()
	{
		return _parent;
	}
	bool XMLNode::IsAttribute(const wstring& name)
	{
		map<wstring, wstring>::const_iterator iter = _attr.find(name);
		return (iter != _attr.end());
	}
	wstring XMLNode::GetAttribute(const wstring& name)
	{
		map<wstring, wstring>::const_iterator iter = _attr.find(name);
		return (iter != _attr.end()) ? iter->second : L"";
	}
	uint XMLNode::GetChildrenCount()
	{
		return (uint)_children.size();
	}
	XMLNode *XMLNode::GetChild(uint num)
	{
		if(num < _children.size())
			return _children[num];
		return nullptr;
	}
}