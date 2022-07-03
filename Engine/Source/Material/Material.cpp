#include "stdafx.h"
#include "Render/Header.h"
#include "Material/Material.h"

namespace Led
{
	VertexShader::VertexShader()
	{
		_type = SHADER_VS;
	}
	VertexShader::~VertexShader()
	{
	}
	bool VertexShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMemory(data, entry, "vs_" + version))
			return false;
		return true;
	}
	bool VertexShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, entry, "vs_" + version))
			return false;
		return true;
	}
	PixelShader::PixelShader()
	{
		_type = SHADER_PS;
	}
	PixelShader::~PixelShader()
	{
	}
	bool PixelShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMemory(data, entry, "ps_" + version))
			return false;
		return true;
	}
	bool PixelShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, entry, "ps_" + version))
			return false;
		return true;
	}
	GeometryShader::GeometryShader()
	{
		_type = SHADER_GS;
	}
	GeometryShader::~GeometryShader()
	{
	}
	bool GeometryShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMemory(data, entry, "gs_" + version))
			return false;
		return true;
	}
	bool GeometryShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, entry, "gs_" + version))
			return false;
		return true;
	}
	HullShader::HullShader()
	{
		_type = SHADER_HS;
	}
	HullShader::~HullShader()
	{
	}
	bool HullShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMemory(data, entry, "hs_" + version))
			return false;
		return true;
	}
	bool HullShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, entry, "hs_" + version))
			return false;
		return true;
	}
	DomainShader::DomainShader()
	{
		_type = SHADER_DS;
	}
	DomainShader::~DomainShader()
	{
	}
	bool DomainShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMemory(data, entry, "ds_" + version))
			return false;
		return true;
	}
	bool DomainShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, entry, "ds_" + version))
			return false;
		return true;
	}

	Material::Material()
	{
	
	}
	Material::~Material()
	{
	
	}
}