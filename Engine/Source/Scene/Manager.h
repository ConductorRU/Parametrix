#pragma once
namespace Led
{

	class Manager
	{
	private:
		static Manager* _this;
		set<InputLayout*> _inputLayouts;
		set<class Material*> _materials;
		map<string,  Material*> _mMaterials;
		set<class VertexShader*> _vs;
		set<class PixelShader*> _ps;
	public:
		static Manager* Get() { return _this; };
		Manager();
		~Manager();
		InputLayout* CreateInputLayout();
		Material* CreateMaterial();
		Material* CreateMaterial(const string &name);
		Material* GetMaterial(const string &name);
		VertexShader* CreateVertexShader();
		PixelShader* CreatePixelShader();

	};
}