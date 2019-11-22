#pragma once
namespace Led
{

	class Manager
	{
	private:
		static Manager* _this;
		set<InputLayout*> _inputLayouts;
		set<class Material*> _materials;
		set<class VertexShader*> _vs;
		set<class PixelShader*> _ps;
	public:
		static Manager* Get() { return _this; };
		Manager();
		~Manager();
		class InputLayout* CreateInputLayout();
		class Material* CreateMaterial();
		class VertexShader* CreateVertexShader();
		class PixelShader* CreatePixelShader();

	};
}