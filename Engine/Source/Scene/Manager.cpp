#include "stdafx.h"
#include "../Render/Header.h"
#include "../Material/Header.h"
#include "Manager.h"

namespace Led
{
	Manager *Manager::_this = nullptr;
	Manager::Manager()
	{
		_this = this;
	}
	Manager::~Manager()
	{
		for(InputLayout* il: _inputLayouts)
			delete il;
		for(Material* mat: _materials)
			delete mat;
		for(VertexShader* vs: _vs)
			delete vs;
		for(PixelShader* ps: _ps)
			delete ps;
	}
	InputLayout* Manager::CreateInputLayout()
	{
		InputLayout* il = new InputLayout();
		_inputLayouts.insert(il);
		return il;
	}
	Material* Manager::CreateMaterial()
	{
		Material* mat = new Material();
		_materials.insert(mat);
		return mat;
	}
	VertexShader* Manager::CreateVertexShader()
	{
		VertexShader* shader = new VertexShader();
		_vs.insert(shader);
		return shader;
	}
	PixelShader* Manager::CreatePixelShader()
	{
		PixelShader* shader = new PixelShader();
		_ps.insert(shader);
		return shader;
	}
}