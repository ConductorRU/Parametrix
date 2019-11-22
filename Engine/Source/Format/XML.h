#pragma once
namespace Led
{
	class XMLNode
	{
	private:
		XMLNode* _parent;
		vector<XMLNode*> _children;
		wstring _name;
		map<wstring, wstring> _attr;
	public:
		XMLNode();
		~XMLNode();
		void SetName(const wstring& name);
		void AddChild(XMLNode *child);
		void SetAttribute(const wstring& name, const wstring& value);
		wstring GetName();
		XMLNode* GetParent();
		bool IsAttribute(const wstring& name);
		wstring GetAttribute(const wstring& name);
		uint GetChildrenCount();
		XMLNode *GetChild(uint num);
	};
	class FormatXML
	{
	
	};
}