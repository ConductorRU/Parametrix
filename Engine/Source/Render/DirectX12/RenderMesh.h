#pragma once
namespace Led
{
	enum SHADER_TYPE
	{
		SHADER_VS = 0,
		SHADER_PS = 1,
		SHADER_GS = 2,
		SHADER_HS = 3,
		SHADER_DS = 4,
	};
	class InputLayout
	{
		friend class RenderMaterial;
	public:
		enum INPUT_LAYOUT
		{
			POSITION    = 0,//float3
			NORMAL      = 1,//float3
			COLOR       = 2,//float4
			TEXCOORD0   = 3,//float2
			TANGENT     = 4,//float3
			BINORMAL    = 5,//float3
			WEIGHT      = 6,//float4
			COUNT       = 7,
		};
	private:
		vector<D3D12_INPUT_ELEMENT_DESC> _layout;
		D3D12_INPUT_LAYOUT_DESC _layoutDesc;
		uint _size;
		uint _offsets[COUNT + 1] = {};
	public:
		static UINT GetElementSize(DXGI_FORMAT format);
		__inline uint GetOffset(INPUT_LAYOUT type) { return _offsets[type]; };
		__inline uint IsOffset(INPUT_LAYOUT type) { return _offsets[type] != 0xffffffff; };
		uint GetCount() { return (uint)_layout.size(); };
		uint GetSize() { return _size; };
		InputLayout();
		~InputLayout();
		void Add(LPCSTR name, uint index, DXGI_FORMAT format, uint slot, uint offset, D3D12_INPUT_CLASSIFICATION iClass, uint stepRate);
		void Add(LPCSTR name, DXGI_FORMAT format, INPUT_LAYOUT type = COUNT);
		class Vertex* CreateVertex();

	};
	class RenderShader
	{
		friend class RenderMaterial;
		friend class RenderMesh;
		friend class ConstantBuffer;
	private:
		struct ShaderValue
		{
			uint ofsset;
			uint size;
		};
		struct ShaderConst
		{
			uint id;
			map<string, ShaderValue> vals;
			uint size;
		};
		ID3DBlob *_shader;
		ID3D10Blob *_info;
		D3D12_SHADER_BYTECODE _bytecode;
		ID3D12ShaderReflection* _reflect;
		map<string, ShaderConst*> _vals;
		vector<ShaderConst*> _valsList;
		D3D12_SHADER_DESC _desk;
		uint _constSize;
		bool Reflect();
	protected:
		SHADER_TYPE _type;
	public:
		RenderShader();
		~RenderShader();
		bool CompileFromMemory(const string &data, const string &entry, const string &target, const D3D_SHADER_MACRO* pDefines = nullptr, ID3DInclude* pInclude = nullptr);
		bool CompileFromFile(const wstring &name, const string &entry, const string &target, const D3D_SHADER_MACRO* pDefines = nullptr, ID3DInclude* pInclude = nullptr);
		bool GetConstant(const string &buffer, const string &name, uint &id, uint &offset, uint &size);
	};
	class RenderTexture
	{
	private:
		ID3D12Resource* _buffer;
		ID3D12DescriptorHeap* _srvHeap;
		ID3D12DescriptorHeap* _samplerHeap;
		D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc;
		D3D12_SAMPLER_DESC _samplerDesc;
		D3D12_DESCRIPTOR_HEAP_DESC _heap;
	public:
		RenderTexture();
		~RenderTexture();
		void Create(uint width, uint height);
		void SetRaw(char* data, uint width, uint height);
	};
	class RenderMaterial
	{
		friend class RenderMesh;
	private:
		ID3D12PipelineState* _pipeline;
		ID3D12RootSignature* _rootSignature;
		ID3DBlob* _signature;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC _desc;
		RenderShader *_shaders[5];
	public:
		RenderMaterial();
		~RenderMaterial();
		void SetInputLayout(class InputLayout *il);
		void SetVertexShader(class RenderShader *shader);
		void SetPixelShader(RenderShader *shader);
		void SetGeometryShader(RenderShader *shader);
		void SetHullShader(RenderShader *shader);
		void SetDomainShader(RenderShader *shader);
		void SetDepthEnable(bool enable);
		void SetDepthFunc(UINT func);
		void SetDepthWriteMask(UINT mask);
		void SetStencilEnable(bool enable);
		void SetPrimitiveTopologyType(UINT type);
		void SetFillMode(UINT mode);
		void SetCullMode(UINT mode);
		void SetDepthBias(int value);
		void SetDepthBiasClamp(float value);
		void SetSlopeScaledDepthBias(float value);
		void SetDepthClip(bool enable);
		void SetMultisample(bool enable);
		void SetAntialiasedLine(bool enable);
		void SetForcedSample(UINT count);
		void SetConservativeRaster(UINT mode);
		void SetBlendEnable(bool enable);
		void SetBlendOp(UINT op);
		void SetBlendOpAlpha(UINT op);
		void SetBlend(UINT srcBlend, UINT dstBlend);
		void SetSrcBlend(UINT blend);
		void SetDestBlend(UINT blend);
		void SetBlendAlpha(UINT srcBlend, UINT dstBlend);
		void SetSrcBlendAlpha(UINT blend);
		void SetDestBlendAlpha(UINT blend);
		void Prepare();
		void Render();
	};

	class Vertex
	{
		friend class RenderMesh;
		friend class InputLayout;
	private:
		char *_data;
		InputLayout *_layout;
		uint _index;
	public:
		uint GetIndex() const { return _index; };
		~Vertex();
		void Set(uint offset, const struct float3 &data);
		void Set(uint offset, const struct Color &data);
		void SetPosition(const float3 &pos);
		void SetPosition2D(const struct float2 &pos);
		void SetPosition2D(const float u, const float v);
		void SetNormal(const float3 &nor);
		void SetColor(const Color &color);
		void SetUV(const float2 &uv);
		void SetUV(const float u, const float v);
		float3 GetPosition() const;
		float3 GetNormal() const;
		Color GetColor() const;
		float2 GetUV() const;
	};

	class ConstantBuffer
	{
	private:
		vector<ID3D12DescriptorHeap*> _cbvHeap;
		vector<ID3D12Resource*> _cbvUploadHeap;
		vector<uchar*> _begin;
		RenderShader *_shader;
		bool _isMap;
		void Map();
		void Unmap();
	public:
		ConstantBuffer(RenderShader *shader);
		~ConstantBuffer();
		void Copy(void *data, UINT size, const string &bufferName, const string &bufferField);
		void Copy(void *data, UINT size, UINT offset = 0);
		void Render(uint num);
		//void SetTexture(class RenderTexture *tex, UINT slot);
	};

	class RenderMesh
	{
	private:
		ID3D12Resource* _buffer;
		ID3D12Resource* _bufferHeap;
		ID3D12Resource* _indexBuffer;
		ID3D12Resource* _indexBufferHeap;
		D3D12_VERTEX_BUFFER_VIEW _view;
		D3D12_INDEX_BUFFER_VIEW _iview;
		D3D_PRIMITIVE_TOPOLOGY _topology;
		InputLayout* _inputLayout;
		bool _isDataUpdate;
		void _Prepare();
		bool _isCPU;
		ConstantBuffer *_constBuffer[5];
	protected:
		vector<Vertex*> _vertex;
		vector<uint> _index;
		void SetMaterial(RenderMaterial *mat);
	public:
		InputLayout *GetInputLayout() { return _inputLayout; };
		RenderMesh(InputLayout* il);
		~RenderMesh();
		Vertex *CreateVertex();
		uint AddIndex(uint i0);
		uint AddTriangle(uint i0, uint i1, uint i2);
		uint AddQuad(uint i0, uint i1, uint i2, uint i3);
		void UpdateBuffer();
		void Render(RenderMaterial *mat);
		void SetTopology(D3D_PRIMITIVE_TOPOLOGY type);
		void SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, uchar* data, uint size);
		void SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const struct float3& data);
		void SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const struct Vector4& data);
		void SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const struct Color& data);
		void SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const struct Matrix& data);
		void SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const struct Matrix4& data);

	};
}