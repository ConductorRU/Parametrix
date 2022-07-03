#pragma once
namespace Led
{
	class VertexShader: public RenderShader
	{
	public:
		VertexShader();
		~VertexShader();
		bool Compile(const string &data, const string &entry = "main", const string &version = "5_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "5_0");
	};
	class PixelShader: public RenderShader
	{
	public:
		PixelShader();
		~PixelShader();
		bool Compile(const string &data, const string &entry = "main", const string &version = "5_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "5_0");
	};
	class GeometryShader: public RenderShader
	{
	public:
		GeometryShader();
		~GeometryShader();
		bool Compile(const string &data, const string &entry = "main", const string &version = "5_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "5_0");
	};
	class HullShader: public RenderShader
	{
	public:
		HullShader();
		~HullShader();
		bool Compile(const string &data, const string &entry = "main", const string &version = "5_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "5_0");
	};
	class DomainShader: public RenderShader
	{
	public:
		DomainShader();
		~DomainShader();
		bool Compile(const string &data, const string &entry = "main", const string &version = "5_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "5_0");
	};

	class Material: public RenderMaterial
	{
	public:
		Material();
		~Material();
	};
}