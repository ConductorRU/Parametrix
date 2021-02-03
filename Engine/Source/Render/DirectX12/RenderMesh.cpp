#include "stdafx.h"
#include "../../Core/Debug.h"
#include "../../Math/Transform.h"
#include "../../Math/Color.h"
#include "RenderMesh.h"
#include "RenderDX12.h"
namespace Led
{
	UINT InputLayout::GetElementSize(DXGI_FORMAT format)
	{
		switch(format)
		{
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_SINT:
			return 16;

		case DXGI_FORMAT_R32G32B32_TYPELESS:
		case DXGI_FORMAT_R32G32B32_FLOAT:
		case DXGI_FORMAT_R32G32B32_UINT:
		case DXGI_FORMAT_R32G32B32_SINT:
			return 12;

		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_UNORM:
		case DXGI_FORMAT_R16G16B16A16_UINT:
		case DXGI_FORMAT_R16G16B16A16_SNORM:
		case DXGI_FORMAT_R16G16B16A16_SINT:
		case DXGI_FORMAT_R32G32_TYPELESS:
		case DXGI_FORMAT_R32G32_FLOAT:
		case DXGI_FORMAT_R32G32_UINT:
		case DXGI_FORMAT_R32G32_SINT:
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return 8;

		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R11G11B10_FLOAT:
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_SINT:
		case DXGI_FORMAT_R16G16_TYPELESS:
		case DXGI_FORMAT_R16G16_FLOAT:
		case DXGI_FORMAT_R16G16_UNORM:
		case DXGI_FORMAT_R16G16_UINT:
		case DXGI_FORMAT_R16G16_SNORM:
		case DXGI_FORMAT_R16G16_SINT:
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_UINT:
		case DXGI_FORMAT_R32_SINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM:
			return 4;

		case DXGI_FORMAT_R8G8_TYPELESS:
		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R8G8_UINT:
		case DXGI_FORMAT_R8G8_SNORM:
		case DXGI_FORMAT_R8G8_SINT:
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_SNORM:
		case DXGI_FORMAT_R16_SINT:
		case DXGI_FORMAT_B5G6R5_UNORM:
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			return 2;

		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_SINT:
		case DXGI_FORMAT_A8_UNORM:
			return 1;
		case DXGI_FORMAT_BC2_TYPELESS:
		case DXGI_FORMAT_BC2_UNORM:
		case DXGI_FORMAT_BC2_UNORM_SRGB:
		case DXGI_FORMAT_BC3_TYPELESS:
		case DXGI_FORMAT_BC3_UNORM:
		case DXGI_FORMAT_BC3_UNORM_SRGB:
		case DXGI_FORMAT_BC5_TYPELESS:
		case DXGI_FORMAT_BC5_UNORM:
		case DXGI_FORMAT_BC5_SNORM:
			return 128;
		case DXGI_FORMAT_R1_UNORM:
		case DXGI_FORMAT_BC1_TYPELESS:
		case DXGI_FORMAT_BC1_UNORM:
		case DXGI_FORMAT_BC1_UNORM_SRGB:
		case DXGI_FORMAT_BC4_TYPELESS:
		case DXGI_FORMAT_BC4_UNORM:
		case DXGI_FORMAT_BC4_SNORM:
			return 8;
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
			return 4;
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
			return 4;

		case DXGI_FORMAT_UNKNOWN:
		default:
			return 0;
		}
	}
	InputLayout::InputLayout()
	{
		_size = 0;
		_layoutDesc = {};
	}
	InputLayout::~InputLayout()
	{
		memset(_offsets, 0xff, sizeof(_offsets));
	}
	void InputLayout::Add(LPCSTR name, uint index, DXGI_FORMAT format, uint slot, uint offset, D3D12_INPUT_CLASSIFICATION iClass, uint stepRate)
	{
		D3D12_INPUT_ELEMENT_DESC d = {name, index, format, slot, offset, iClass, stepRate};
		_size += GetElementSize(format);
		_layout.push_back(d);
		_layoutDesc.NumElements = (uint)_layout.size();
		_layoutDesc.pInputElementDescs = &_layout[0];
	}
	void InputLayout::Add(LPCSTR name, DXGI_FORMAT format, INPUT_LAYOUT type)
	{
		D3D12_INPUT_ELEMENT_DESC d = {name, 0, format, 0, _size, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
		if(type != COUNT)
			_offsets[type] = _size;
		_size += GetElementSize(format);
		_layout.push_back(d);
		_layoutDesc.NumElements = (uint)_layout.size();
		_layoutDesc.pInputElementDescs = &_layout[0];
	}

	Vertex* InputLayout::CreateVertex()
	{
		Vertex *v = new Vertex;
		v->_data = new char[_size];
		memset(v->_data, 0, _size);
		v->_layout = this;
		return v;
	}

	RenderShader::RenderShader()
	{
		_shader = nullptr;
		_info = nullptr;
		_reflect = nullptr;
		_desk = {};
		_bytecode = {};
		_constSize = 0;
		_type = SHADER_VS;
	}
	RenderShader::~RenderShader()
	{
		for(ShaderConst* shader: _valsList)
			delete shader;
		if(_shader)
			_shader->Release();
		if(_info)
			_info->Release();
	}
	bool RenderShader::Reflect()
	{
		HRESULT hr = D3DReflect(_shader->GetBufferPointer(), _shader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&_reflect);
		if(hr != S_OK)
			return false;
		_reflect->GetDesc(&_desk);
		ID3D12ShaderReflectionConstantBuffer *hh = nullptr;
		ID3D12ShaderReflectionVariable *var = nullptr;
		D3D12_SHADER_BUFFER_DESC pDesc;
		D3D12_SHADER_VARIABLE_DESC vDesc;
		uint rSize = 0;
		for(uint i = 0; i != _desk.ConstantBuffers; ++i)
		{
			hh = _reflect->GetConstantBufferByIndex(i);
			hh->GetDesc(&pDesc);
			UINT varCount = pDesc.Variables;
			ShaderConst *cn = new ShaderConst();
			cn->id = i;
			cn->size = pDesc.Size;
			for(UINT v = 0; v != varCount; ++v)
			{
				var = hh->GetVariableByIndex(v);
				var->GetDesc(&vDesc);
				cn->vals[vDesc.Name] = {vDesc.StartOffset, vDesc.Size};
				_constSize = vDesc.StartOffset + vDesc.Size;
			}
			_vals[pDesc.Name] = cn;
			_valsList.push_back(cn);
		}
		return true;
	}
	bool RenderShader::CompileFromMemory(const string &data, const string &entry, const string &target, const D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude)
	{
		_bytecode = {};
#ifdef _DEBUG
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;// | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
		UINT compileFlags = 0;
#endif
		HRESULT hr = D3DCompile(data.c_str(), data.size(), NULL, pDefines, pInclude, entry.c_str(), target.c_str(), compileFlags, 0, &_shader, &_info);
		if(hr != S_OK)
		{
			Debug::Log((char*)_info->GetBufferPointer());
			_info->Release();
			return false;
		}
		Reflect();
		_bytecode.BytecodeLength = _shader->GetBufferSize();
		_bytecode.pShaderBytecode = _shader->GetBufferPointer();
		return true;
	}
	bool RenderShader::CompileFromFile(const wstring &name, const string &entry, const string &target, const D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude)
	{
		_bytecode = {};
#ifdef _DEBUG
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;// | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
		UINT compileFlags = 0;
#endif
		HRESULT hr = D3DCompileFromFile(name.c_str(), pDefines, pInclude, entry.c_str(), target.c_str(), compileFlags, 0, &_shader, &_info);
		if(FAILED(hr))
		{
			Debug::Log((char*)_info->GetBufferPointer());
			_info->Release();
			return false;
		}
		Reflect();
		_bytecode.BytecodeLength = _shader->GetBufferSize();
		_bytecode.pShaderBytecode = _shader->GetBufferPointer();
		return true;
	}

	bool RenderShader::GetConstant(const string &buffer, const string &name, uint &id, uint &offset, uint &size)
	{
		auto it = _vals.find(buffer);
		if(it == _vals.end())
			return false;
		auto it2 = it->second->vals.find(name);
		if(it2 == it->second->vals.end())
			return false;
		offset = it2->second.ofsset;
		size = it2->second.size;
		id = it->second->id;
		return true;
	}

	RenderTexture::RenderTexture()
	{
		_buffer = nullptr;
		_srvHeap = nullptr;
		_samplerHeap = nullptr;
		_srvDesc = {};
		_samplerDesc = {};
		_heap = {};
	}
	RenderTexture::~RenderTexture()
	{
		if(_srvHeap)
			_srvHeap->Release();
		if(_samplerHeap)
			_samplerHeap->Release();
		if(_buffer)
			_buffer->Release();
	}
	void RenderTexture::Create(uint width, uint height)
	{
		/*
		RenderDX12 *ren = RenderDX12::Get();
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_B8G8R8A8_UNORM, width, height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT_UNKNOWN, 0);
		D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER, 0, width*height*4, 1, 1, 1, DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_RESOURCE_FLAG_NONE);
		Debug::ThrowIfFailed(ren->_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_buffer)));

		_buffer->SetName(L"Texure");
		char *c = new char[width*height*4];
		memset(c, 255, width*height * 4);
		SetRaw(c, width, height);
		delete[] c;

		D3D12_DESCRIPTOR_HEAP_DESC desc;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = 10;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
		Debug::ThrowIfFailed(ren->_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_srvHeap)));

		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		desc.NumDescriptors = 10;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
		Debug::ThrowIfFailed(ren->_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_samplerHeap)));

		_srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		_srvDesc.Texture2D.MipLevels = 1; // No MIP
		_srvDesc.Texture2D.MostDetailedMip = 0;
		_srvDesc.Texture2D.PlaneSlice = 0;
		_srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

		auto start = _srvHeap->GetCPUDescriptorHandleForHeapStart().ptr;
		auto step = ren->_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		start += step*(slot + 1);
		ren->_device->CreateShaderResourceView(_buffer, &_srvDesc, {start});

		_samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		_samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		_samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		_samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		_samplerDesc.MinLOD = -FLT_MAX;
		_samplerDesc.MaxLOD = FLT_MAX;
		_samplerDesc.MipLODBias = 0;
		_samplerDesc.MaxAnisotropy = 0;
		_samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		start = _samplerHeap->GetCPUDescriptorHandleForHeapStart().ptr;
		step = ren->_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		start += step*(slot);
		ren->_device->CreateSampler(&_samplerDesc, {start});*/
	}
	void RenderTexture::SetRaw(char *data, UINT width, UINT height)
	{
		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = data;
		textureData.RowPitch = width * 4;
		textureData.SlicePitch = textureData.RowPitch * height;
		Render *ren = Render::Get();
		BYTE* pData;
		_buffer->Map(0, NULL, reinterpret_cast<void**>(&pData));
		memcpy(pData, data, width*height * 4);
		_buffer->Unmap(0, NULL);
	}
	RenderMaterial::RenderMaterial()
	{
		_pipeline = nullptr;
		_desc = {};
		_rootSignature = nullptr;
		_signature = nullptr;
		uint sof = sizeof(_shaders);
		memset(_shaders, 0, sizeof(_shaders));
		_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    _desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    _desc.RasterizerState.FrontCounterClockwise = FALSE;
    _desc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    _desc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    _desc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    _desc.RasterizerState.DepthClipEnable = TRUE;
    _desc.RasterizerState.MultisampleEnable = FALSE;
    _desc.RasterizerState.AntialiasedLineEnable = FALSE;
    _desc.RasterizerState.ForcedSampleCount = 0;
    _desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		_desc.BlendState.AlphaToCoverageEnable = FALSE;
		_desc.BlendState.IndependentBlendEnable = FALSE;

		D3D12_RENDER_TARGET_BLEND_DESC defaultRT = {};
		defaultRT.BlendEnable = FALSE;
    defaultRT.LogicOpEnable = FALSE;
    defaultRT.SrcBlend = D3D12_BLEND_ONE;
    defaultRT.DestBlend = D3D12_BLEND_ZERO;
    defaultRT.BlendOp = D3D12_BLEND_OP_ADD;
    defaultRT.SrcBlendAlpha = D3D12_BLEND_ONE;
    defaultRT.DestBlendAlpha = D3D12_BLEND_ZERO;
    defaultRT.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    defaultRT.LogicOp = D3D12_LOGIC_OP_NOOP;
    defaultRT.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		for(UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			_desc.BlendState.RenderTarget[i] = defaultRT;

		_desc.DepthStencilState.DepthEnable = true;
		_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		_desc.DepthStencilState.StencilEnable = FALSE;
		_desc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		_desc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
		D3D12_DEPTH_STENCILOP_DESC defaultStencilOp = { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
		_desc.DepthStencilState.BackFace = _desc.DepthStencilState.FrontFace = defaultStencilOp;

		_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		_desc.SampleMask = UINT_MAX;
		_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		_desc.NumRenderTargets = 1;
		_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_desc.SampleDesc.Count = 1;
		//_desc.SampleDesc.Quality = 16;
	}
	RenderMaterial::~RenderMaterial()
	{
		if(_pipeline)
			_pipeline->Release();
		if(_signature)
			_signature->Release();
		if(_rootSignature)
			_rootSignature->Release();
	}
	void RenderMaterial::SetInputLayout(InputLayout *il)
	{
		_desc.InputLayout = il->_layoutDesc;
	}
	void RenderMaterial::SetVertexShader(RenderShader *shader)
	{
		_desc.VS = shader ? shader->_bytecode : D3D12_SHADER_BYTECODE{nullptr, 0};
		_shaders[0] = shader;
	}
	void RenderMaterial::SetPixelShader(RenderShader *shader)
	{
		_desc.PS = shader ? shader->_bytecode : D3D12_SHADER_BYTECODE{nullptr, 0};
		_shaders[1] = shader;
	}
	void RenderMaterial::SetGeometryShader(RenderShader *shader)
	{
		_desc.GS = shader ? shader->_bytecode : D3D12_SHADER_BYTECODE{nullptr, 0};
		_shaders[2] = shader;
	}
	void RenderMaterial::SetHullShader(RenderShader *shader)
	{
		_desc.HS = shader ? shader->_bytecode : D3D12_SHADER_BYTECODE{nullptr, 0};
		_shaders[3] = shader;
	}
	void RenderMaterial::SetDomainShader(RenderShader *shader)
	{
		_desc.DS = shader ? shader->_bytecode : D3D12_SHADER_BYTECODE{nullptr, 0};
		_shaders[4] = shader;
	}
	void RenderMaterial::SetDepthEnable(bool enable)
	{
		_desc.DepthStencilState.DepthEnable = enable;
	}
	void RenderMaterial::SetDepthFunc(UINT func)
	{
		_desc.DepthStencilState.DepthFunc = (D3D12_COMPARISON_FUNC)func;
	}
	void RenderMaterial::SetDepthWriteMask(UINT mask)
	{
		_desc.DepthStencilState.DepthWriteMask = (D3D12_DEPTH_WRITE_MASK)mask;
	}
	void RenderMaterial::SetStencilEnable(bool enable)
	{
		_desc.DepthStencilState.StencilEnable = enable;
	}
	void RenderMaterial::SetPrimitiveTopologyType(UINT type)
	{
		_desc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)type;
	}
	void RenderMaterial::SetFillMode(UINT mode)
	{
		_desc.RasterizerState.FillMode = (D3D12_FILL_MODE)mode;
	}
	void RenderMaterial::SetCullMode(UINT mode)
	{
		_desc.RasterizerState.CullMode = (D3D12_CULL_MODE)mode;
	}
	void RenderMaterial::SetDepthBias(int value)
	{
		_desc.RasterizerState.DepthBias = value;
	}
	void RenderMaterial::SetDepthBiasClamp(float value)
	{
		_desc.RasterizerState.DepthBiasClamp = value;
	}
	void RenderMaterial::SetSlopeScaledDepthBias(float value)
	{
		_desc.RasterizerState.SlopeScaledDepthBias = value;
	}
	void RenderMaterial::SetDepthClip(bool enable)
	{
		_desc.RasterizerState.DepthClipEnable = enable;
	}
	void RenderMaterial::SetMultisample(bool enable)
	{
		_desc.RasterizerState.MultisampleEnable = enable;
	}
	void RenderMaterial::SetAntialiasedLine(bool enable)
	{
		_desc.RasterizerState.AntialiasedLineEnable = enable;
	}
	void RenderMaterial::SetForcedSample(UINT count)
	{
		_desc.RasterizerState.ForcedSampleCount = count;
	}
	void RenderMaterial::SetConservativeRaster(UINT mode)
	{
		_desc.RasterizerState.ConservativeRaster = (D3D12_CONSERVATIVE_RASTERIZATION_MODE)mode;
	}

	void RenderMaterial::SetBlendEnable(bool enable)
	{
		for(UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			_desc.BlendState.RenderTarget[i].BlendEnable = (BOOL)enable;
	}

	void RenderMaterial::SetBlendOp(UINT op)
	{
		for(UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			_desc.BlendState.RenderTarget[i].BlendOp = (D3D12_BLEND_OP)op;
	}

	void RenderMaterial::SetBlendOpAlpha(UINT op)
	{
		for(UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			_desc.BlendState.RenderTarget[i].BlendOpAlpha = (D3D12_BLEND_OP)op;
	}

	void RenderMaterial::SetBlend(UINT srcBlend, UINT dstBlend)
	{
		SetSrcBlend(srcBlend);
		SetDestBlend(dstBlend);
	}

	void RenderMaterial::SetSrcBlend(UINT blend)
	{
		for(UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			_desc.BlendState.RenderTarget[i].SrcBlend = (D3D12_BLEND)blend;
	}

	void RenderMaterial::SetDestBlend(UINT blend)
	{
		for(UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			_desc.BlendState.RenderTarget[i].DestBlend = (D3D12_BLEND)blend;
	}

	void RenderMaterial::SetBlendAlpha(UINT srcBlend, UINT dstBlend)
	{
		SetSrcBlendAlpha(srcBlend);
		SetDestBlendAlpha(dstBlend);
	}

	void RenderMaterial::SetSrcBlendAlpha(UINT blend)
	{
		for(UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			_desc.BlendState.RenderTarget[i].SrcBlendAlpha = (D3D12_BLEND)blend;
	}

	void RenderMaterial::SetDestBlendAlpha(UINT blend)
	{
		for(UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			_desc.BlendState.RenderTarget[i].DestBlendAlpha = (D3D12_BLEND)blend;
	}

	void RenderMaterial::Prepare()
	{
		if(_pipeline)
			_pipeline->Release();
		_pipeline = nullptr;

		vector<D3D12_ROOT_PARAMETER> rootParameters;
		vector<D3D12_DESCRIPTOR_RANGE> ranges;
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
		
		if(_desc.VS.pShaderBytecode && _shaders[0]->_desk.ConstantBuffers || _desc.PS.pShaderBytecode && _shaders[1]->_desk.ConstantBuffers)
		{
			// create a descriptor range (descriptor table) and fill it out this is a range of descriptors inside a descriptor heap
			D3D12_DESCRIPTOR_RANGE range; // only one range right now
			range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // this is a range of constant buffer views (descriptors)
			range.NumDescriptors = 0; // we only have one constant buffer, so the range is only 1
			range.BaseShaderRegister = 0; // start index of the shader registers in the range
			range.RegisterSpace = 0; // space 0. can usually be zero
			range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // this appends the range to the end of the root signature descriptor tables
			ranges.push_back(range);
			// create a descriptor table
			D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
			descriptorTable.NumDescriptorRanges = (uint)ranges.size(); // we only have one range
			descriptorTable.pDescriptorRanges = &ranges[0]; // the pointer to the beginning of our ranges array

			if(_desc.VS.pShaderBytecode && _shaders[0]->_desk.ConstantBuffers)
			{
				D3D12_ROOT_PARAMETER rootParameter = {};
				rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // this is a descriptor table
				rootParameter.DescriptorTable = descriptorTable; // this is our descriptor table for this root parameter
				rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // our pixel shader will be the only shader accessing this parameter for now
				rootParameters.push_back(rootParameter);
				rootSignatureDesc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
				ranges[0].NumDescriptors += 1;
			}
			if(_desc.PS.pShaderBytecode && _shaders[1]->_desk.ConstantBuffers)
			{
				D3D12_ROOT_PARAMETER rootParameter = {};
				rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // this is a descriptor table
				rootParameter.DescriptorTable = descriptorTable; // this is our descriptor table for this root parameter
				rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // our pixel shader will be the only shader accessing this parameter for now
				rootParameters.push_back(rootParameter);
				rootSignatureDesc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
				ranges[0].NumDescriptors += 1;
			}
		}
		/*if(_desc.VS.pShaderBytecode && _shaders[0]->_desk.ConstantBuffers)
		{
			// create a descriptor range (descriptor table) and fill it out this is a range of descriptors inside a descriptor heap
			D3D12_DESCRIPTOR_RANGE  descriptorTableRanges[1]; // only one range right now
			descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // this is a range of constant buffer views (descriptors)
			descriptorTableRanges[0].NumDescriptors = _shaders[0]->_desk.ConstantBuffers; // we only have one constant buffer, so the range is only 1
			descriptorTableRanges[0].BaseShaderRegister = 0; // start index of the shader registers in the range
			descriptorTableRanges[0].RegisterSpace = 0; // space 0. can usually be zero
			descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // this appends the range to the end of the root signature descriptor tables
			// create a descriptor table
			D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
			descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges); // we only have one range
			descriptorTable.pDescriptorRanges = &descriptorTableRanges[0]; // the pointer to the beginning of our ranges array

			D3D12_ROOT_PARAMETER rootParameter;
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // this is a descriptor table
			rootParameter.DescriptorTable = descriptorTable; // this is our descriptor table for this root parameter
			rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // our pixel shader will be the only shader accessing this parameter for now
			rootParameters.push_back(rootParameter);
			rootSignatureDesc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
		}
		if(_desc.PS.pShaderBytecode && _shaders[1]->_desk.ConstantBuffers)
		{
			// create a descriptor range (descriptor table) and fill it out this is a range of descriptors inside a descriptor heap
			D3D12_DESCRIPTOR_RANGE  descriptorTableRanges[1]; // only one range right now
			descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // this is a range of constant buffer views (descriptors)
			descriptorTableRanges[0].NumDescriptors = _shaders[1]->_desk.ConstantBuffers; // we only have one constant buffer, so the range is only 1
			descriptorTableRanges[0].BaseShaderRegister = 1; // start index of the shader registers in the range
			descriptorTableRanges[0].RegisterSpace = 0; // space 0. can usually be zero
			descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // this appends the range to the end of the root signature descriptor tables
			// create a descriptor table
			D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
			descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges); // we only have one range
			descriptorTable.pDescriptorRanges = &descriptorTableRanges[0]; // the pointer to the beginning of our ranges array

			D3D12_ROOT_PARAMETER rootParameter;
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // this is a descriptor table
			rootParameter.DescriptorTable = descriptorTable; // this is our descriptor table for this root parameter
			rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // our pixel shader will be the only shader accessing this parameter for now
			rootParameters.push_back(rootParameter);
			rootSignatureDesc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
		}*/
		rootSignatureDesc.NumParameters = (uint)rootParameters.size();
    rootSignatureDesc.pParameters = rootParameters.size() ? &rootParameters[0] : nullptr;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;


		//D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		//rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &_signature, nullptr);
		Debug::ThrowIfFailed(hr);

		hr = RenderDX12::Get()->_device->CreateRootSignature(0, _signature->GetBufferPointer(), _signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
		Debug::ThrowIfFailed(hr);

		_desc.pRootSignature = _rootSignature;
		hr = RenderDX12::Get()->_device->CreateGraphicsPipelineState(&_desc, IID_PPV_ARGS(&_pipeline));
		Debug::ThrowIfFailed(hr);
	}
	void RenderMaterial::Render()
	{
		RenderDX12::Get()->_commandList->SetGraphicsRootSignature(_rootSignature); // set the root signature
		RenderDX12::Get()->_commandList->SetPipelineState(_pipeline);
	}
	Vertex::~Vertex()
	{
		delete[] _data;
	}
	void Vertex::Set(uint offset, const float3 &data)
	{
		memcpy(&_data[offset], &data, sizeof(data));
	}
	void Vertex::Set(uint offset, const Color &data)
	{
		memcpy(&_data[offset], &data, sizeof(data));
	}
	void Vertex::SetPosition(const float3 &pos)
	{
		if(_layout->IsOffset(InputLayout::POSITION))
			memcpy(&_data[_layout->GetOffset(InputLayout::POSITION)], &pos, sizeof(pos));
	}

	void Vertex::SetPosition2D(const float2 &pos)
	{
		if(_layout->IsOffset(InputLayout::POSITION))
			memcpy(&_data[_layout->GetOffset(InputLayout::POSITION)], &pos, sizeof(pos));
	}

	void Vertex::SetPosition2D(const float u, const float v)
	{
		if(_layout->IsOffset(InputLayout::POSITION))
		{
			memcpy(&_data[_layout->GetOffset(InputLayout::POSITION)], &u, sizeof(u));
			memcpy(&_data[_layout->GetOffset(InputLayout::POSITION) + sizeof(u)], &v, sizeof(v));
		}
	}

	void Vertex::SetNormal(const float3 &nor)
	{
		if(_layout->IsOffset(InputLayout::NORMAL))
			memcpy(&_data[_layout->GetOffset(InputLayout::NORMAL)], &nor, sizeof(nor));
	}
	void Vertex::SetColor(const Color &color)
	{
		if(_layout->IsOffset(InputLayout::COLOR))
			memcpy(&_data[_layout->GetOffset(InputLayout::COLOR)], &color, sizeof(color));
	}
	void Vertex::SetUV(const float2 &uv)
	{
		if(_layout->IsOffset(InputLayout::TEXCOORD0))
			memcpy(&_data[_layout->GetOffset(InputLayout::TEXCOORD0)], &uv, sizeof(uv));
	}

	void Vertex::SetUV(const float u, const float v)
	{
		if(_layout->IsOffset(InputLayout::TEXCOORD0))
		{
			memcpy(&_data[_layout->GetOffset(InputLayout::TEXCOORD0)], &u, sizeof(u));
			memcpy(&_data[_layout->GetOffset(InputLayout::TEXCOORD0) + sizeof(u)], &v, sizeof(v));
		}
	}

	float3 Vertex::GetPosition() const
	{
		float3 res;
		if(_layout->IsOffset(InputLayout::POSITION))
			memcpy(&res, &_data[_layout->GetOffset(InputLayout::POSITION)], sizeof(float3));
		return res;
	}
	float3 Vertex::GetNormal() const
	{
		float3 res;
		if(_layout->IsOffset(InputLayout::NORMAL))
			memcpy(&res, &_data[_layout->GetOffset(InputLayout::NORMAL)], sizeof(float3));
		return res;
	}
	Color Vertex::GetColor() const
	{
		Color res;
		if(_layout->IsOffset(InputLayout::TEXCOORD0))
			memcpy(&res, &_data[_layout->GetOffset(InputLayout::COLOR)], sizeof(Color));
		return res;
	}
	float2 Vertex::GetUV() const
	{
		float2 res;
		if(_layout->IsOffset(InputLayout::TEXCOORD0))
			memcpy(&res, &_data[_layout->GetOffset(InputLayout::TEXCOORD0)], sizeof(float2));
		return res;
	}

	ConstantBuffer::ConstantBuffer(RenderShader *shader)
	{
		_isMap = false;
		_shader = shader;
		uint size = _shader->_constSize;
		uint frameBufferCount = RenderDX12::Get()->GetFrameBufferCount();
		_cbvHeap.resize(frameBufferCount);
		_cbvUploadHeap.resize(frameBufferCount);
		_begin.resize(frameBufferCount);
		
		
		ID3D12Device* device = RenderDX12::Get()->_device;
		HRESULT hr;
		for(uint i = 0; i < frameBufferCount; ++i)
		{
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
			heapDesc.NumDescriptors = 1;
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_cbvHeap[i]));
			Debug::ThrowIfFailed(hr);
		}
		for(uint i = 0; i < frameBufferCount; ++i)
		{
			D3D12_HEAP_PROPERTIES props;
			props.Type = D3D12_HEAP_TYPE_UPLOAD;
			props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			props.CreationNodeMask = 1;
			props.VisibleNodeMask = 1;

			D3D12_RESOURCE_DESC desc;
			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
      desc.Alignment = 0;
			desc.Width = (size + 1024 * 64 - 1) & ~(1024 * 64 - 1);// size of the resource heap. Must be a multiple of 64KB for single-textures and constant buffers
      desc.Height = 1;
      desc.DepthOrArraySize = 1;
      desc.MipLevels = 1;
      desc.Format = DXGI_FORMAT_UNKNOWN;
      desc.SampleDesc.Count = 1;
      desc.SampleDesc.Quality = 0;
      desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
      desc.Flags = D3D12_RESOURCE_FLAG_NONE;

			hr = device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_cbvUploadHeap[i]));
			_cbvUploadHeap[i]->SetName(L"Constant Buffer Upload Resource Heap");

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = _cbvUploadHeap[i]->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = (size + 255) & ~255;    // CB size is required to be 256-byte aligned.
			device->CreateConstantBufferView(&cbvDesc, _cbvHeap[i]->GetCPUDescriptorHandleForHeapStart());

			D3D12_RANGE range = {0, 0};// We do not intend to read from this resource on the CPU. (End is less than or equal to begin)
			hr = _cbvUploadHeap[i]->Map(0, &range, reinterpret_cast<void**>(&_begin[i]));
			memset(_begin[i], 0, size);
			_cbvUploadHeap[i]->Unmap(0, &range);
		}
	}
	ConstantBuffer::~ConstantBuffer()
	{
		for(ID3D12DescriptorHeap* heap: _cbvHeap)
			heap->Release();
		for(ID3D12Resource* heap: _cbvUploadHeap)
			heap->Release();
	}
	void ConstantBuffer::Map()
	{
		uint frameIndex = RenderDX12::Get()->GetFrameIndex();
		D3D12_RANGE range = {0, 0};// We do not intend to read from this resource on the CPU. (End is less than or equal to begin)
		HRESULT hr = _cbvUploadHeap[frameIndex]->Map(0, &range, reinterpret_cast<void**>(&_begin[frameIndex]));
		_isMap = true;
	}
	void ConstantBuffer::Unmap()
	{
		D3D12_RANGE range = {0, 0};
		_cbvUploadHeap[RenderDX12::Get()->GetFrameIndex()]->Unmap(0, &range);
		_isMap = false;
	}
	void ConstantBuffer::Copy(void *data, UINT size, const string &bufferName, const string &bufferField)
	{
		if(!_isMap)
			Map();
		uint frameIndex = RenderDX12::Get()->GetFrameIndex();
		uint bufId, cSize, offset;
		if(_shader->GetConstant(bufferName, bufferField, bufId, offset, cSize))
			memcpy(&(_begin[frameIndex][offset]), data, size);
	}
	void ConstantBuffer::Copy(void *data, UINT size, UINT offset)
	{
		uint frameIndex = RenderDX12::Get()->GetFrameIndex();
		memcpy(&(_begin[frameIndex][offset]), data, size);
	}
	void ConstantBuffer::Render(uint num)
	{
		if(_isMap)
			Unmap();
		uint frameIndex = RenderDX12::Get()->GetFrameIndex();
		ID3D12DescriptorHeap* descriptorHeaps[] = { _cbvHeap[frameIndex] };
		RenderDX12::Get()->_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		RenderDX12::Get()->_commandList->SetGraphicsRootDescriptorTable(num, _cbvHeap[frameIndex]->GetGPUDescriptorHandleForHeapStart());
	}


	RenderMesh::RenderMesh(InputLayout* il)
	{
		_inputLayout = il;
		_isDataUpdate = true;
		_buffer = nullptr;
		_bufferHeap = nullptr;
		_isCPU = true;
		_indexBuffer = nullptr;
		_indexBufferHeap = nullptr;
		_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		memset(_constBuffer, 0, sizeof(_constBuffer));
	}
	RenderMesh::~RenderMesh()
	{
		for(ConstantBuffer* buf: _constBuffer)
			if(buf)
				delete buf;
		for(Vertex *v: _vertex)
			delete v;
		if(_bufferHeap)
			_bufferHeap->Release();
		if(_buffer)
			_buffer->Release();
		if(_indexBuffer)
			_indexBuffer->Release();
		if(_indexBufferHeap)
			_indexBufferHeap->Release();
	}
	void RenderMesh::SetMaterial(RenderMaterial *mat)
	{
		if(mat)
		{
			for(int i = 0; i < 5; ++i)
			{
				if(_constBuffer[i])
					delete _constBuffer[i];
				if(mat->_shaders[i] && mat->_shaders[i]->_constSize)
					_constBuffer[i] = new ConstantBuffer(mat->_shaders[i]);
			}
		}
	}
	Vertex *RenderMesh::CreateVertex()
	{
		Vertex *v = _inputLayout->CreateVertex();
		v->_index = (uint)_vertex.size();
		_vertex.push_back(v);
		_isDataUpdate = true;
		return v;
	}
	uint RenderMesh::AddIndex(uint i0)
	{
		_index.push_back(i0);
		return (uint)_index.size();
	}
	uint RenderMesh::AddTriangle(uint i0, uint i1, uint i2)
	{
		_index.push_back(i0);
		_index.push_back(i1);
		_index.push_back(i2);
		return (uint)_index.size();
	}
	uint RenderMesh::AddQuad(uint i0, uint i1, uint i2, uint i3)
	{
		_index.push_back(i0);
		_index.push_back(i1);
		_index.push_back(i2);
		_index.push_back(i2);
		_index.push_back(i3);
		_index.push_back(i0);
		return (uint)_index.size();
	}
	
	void RenderMesh::_Prepare()
	{
		if(_bufferHeap)
			_bufferHeap->Release();
		if(_buffer)
			_buffer->Release();
		if(_indexBuffer)
			_indexBuffer->Release();
		if(_indexBufferHeap)
			_indexBufferHeap->Release();
		_bufferHeap = _buffer = _indexBuffer = _indexBufferHeap = nullptr;
		ulong size = _vertex.size()*_inputLayout->GetSize();
		if(!size)
			return;
		D3D12_HEAP_PROPERTIES hProps = {};
		hProps.Type = (_isCPU ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT);
		hProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		hProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		hProps.CreationNodeMask = 1;
		hProps.VisibleNodeMask = 1;
		D3D12_RESOURCE_DESC rDesc = {};
		rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		rDesc.Alignment = 0;
		rDesc.Width = size;
		rDesc.Height = 1;
		rDesc.DepthOrArraySize = 1;
		rDesc.MipLevels = 1;
		rDesc.Format = DXGI_FORMAT_UNKNOWN;
		rDesc.SampleDesc.Count = 1;
		rDesc.SampleDesc.Quality = 0;
		rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		RenderDX12::Get()->_device->CreateCommittedResource(&hProps, D3D12_HEAP_FLAG_NONE, &rDesc, (_isCPU ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COPY_DEST), nullptr, IID_PPV_ARGS(&_buffer));
		_buffer->SetName(L"Vertex Buffer Resource Heap");
		uint vSize = _inputLayout->GetSize();
		if(!_isCPU)
		{
			hProps.Type = D3D12_HEAP_TYPE_UPLOAD;
			RenderDX12::Get()->_device->CreateCommittedResource(&hProps, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_bufferHeap));
			_bufferHeap->SetName(L"Vertex Buffer Upload Resource Heap");

			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = reinterpret_cast<BYTE*>(&_vertex[0]); // pointer to our vertex array
			vertexData.RowPitch = rDesc.Width; // size of all our triangle vertex data
			vertexData.SlicePitch = rDesc.Width; // also the size of our triangle vertex data

			RenderDX12::Get()->UpdateSubresources(_buffer, _bufferHeap, 0, 0, 1, &vertexData);
			RenderDX12::Get()->BarrierTransition(_buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			RenderDX12::Get()->ExecuteCommandList();
			RenderDX12::Get()->Signal();
			RenderDX12::Get()->ResetCommandList();
		}
		else
		{
			UINT8* pVertexDataBegin;
			_buffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin));
			uint s = 0;
			for(Vertex *v: _vertex)
			{
				memcpy(&pVertexDataBegin[s], v->_data, vSize);
				s += vSize;
			}
			_buffer->Unmap(0, nullptr);
		}
		if(_index.size())
		{
			hProps.Type = (_isCPU ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT);
			rDesc.Width = _index.size()*sizeof(uint);
			RenderDX12::Get()->_device->CreateCommittedResource(&hProps, D3D12_HEAP_FLAG_NONE, &rDesc, (_isCPU ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COPY_DEST), nullptr, IID_PPV_ARGS(&_indexBuffer));
			if(!_isCPU)
			{

			}
			else
			{
				UINT8* pIndexDataBegin;
				_indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pIndexDataBegin));
				memcpy(pIndexDataBegin, &_index[0], rDesc.Width);
				_indexBuffer->Unmap(0, nullptr);
			}
		}
		_isDataUpdate = true;
	}
	void RenderMesh::UpdateBuffer()
	{
		if(_isDataUpdate)
			_Prepare();
	}
	void RenderMesh::Render(RenderMaterial* mat)
	{
		if(!_buffer)
			return;
		if(_isDataUpdate)
		{
			_view.BufferLocation = _buffer->GetGPUVirtualAddress();// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
			_view.StrideInBytes = _inputLayout->GetSize();
			_view.SizeInBytes = (uint)_vertex.size()*_inputLayout->GetSize();
			if(_indexBuffer)
			{
				_iview.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
				_iview.SizeInBytes = (uint)_index.size()*sizeof(uint);
				_iview.Format = DXGI_FORMAT_R32_UINT;
			}
			_isDataUpdate = false;
		}
		uint num = 0;
		for(ConstantBuffer* buf: _constBuffer)
			if(buf)
			{
				buf->Render(num);
				++num;
			}
		RenderDX12::Get()->_commandList->IASetPrimitiveTopology(_topology); // set the primitive topology
		RenderDX12::Get()->_commandList->IASetVertexBuffers(0, 1, &_view); // set the vertex buffer (using the vertex buffer view)
		if(_indexBuffer)
		{
			RenderDX12::Get()->_commandList->IASetIndexBuffer(&_iview);
			RenderDX12::Get()->_commandList->DrawIndexedInstanced((uint)_index.size(), 1, 0, 0, 0);
		}
		else
			RenderDX12::Get()->_commandList->DrawInstanced((uint)_vertex.size(), 1, 0, 0);
	}

	void RenderMesh::SetTopology(D3D_PRIMITIVE_TOPOLOGY type)
	{
		_topology = type;
	}

	void RenderMesh::SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, uchar* data, uint size)
	{
		if(_constBuffer[sType])
			_constBuffer[sType]->Copy(data, size, bufferName, bufferField);
	}
	void RenderMesh::SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const float3& data)
	{
		SetConst(sType, bufferName, bufferField, (uchar*)&data, sizeof(float3));
	}

	void RenderMesh::SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const Vector4& data)
	{
		SetConst(sType, bufferName, bufferField, (uchar*)&data, sizeof(Vector4));
	}

	void RenderMesh::SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const Color& data)
	{
		SetConst(sType, bufferName, bufferField, (uchar*)&data, sizeof(Color));
	}
	void RenderMesh::SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const Matrix& data)
	{
		SetConst(sType, bufferName, bufferField, (uchar*)&data, sizeof(Matrix));
	}
	void RenderMesh::SetConst(SHADER_TYPE sType, const string& bufferName, const string& bufferField, const Matrix4& data)
	{
		SetConst(sType, bufferName, bufferField, (uchar*)&data, sizeof(Matrix4));
	}

}