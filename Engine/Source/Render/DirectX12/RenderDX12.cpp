#include "stdafx.h"
#include "Header.h"
#include "../../Core/Debug.h"
#include "../../Core/Window.h"
#include "../../Core/Engine.h"
namespace Led
{
	RenderDX12 *RenderDX12::_this = nullptr;
	DepthStencil::DepthStencil()
	{
		_dsDescriptor = nullptr;
		_dsBuffer = nullptr;
	}
	DepthStencil::~DepthStencil()
	{
		if(_dsBuffer)
			_dsBuffer->Release();
		if(_dsDescriptor)
			_dsDescriptor->Release();
	}
	void DepthStencil::Create(uint width, uint height)
	{
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		HRESULT hr = RenderDX12::Get()->_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&_dsDescriptor));
		if(FAILED(hr))
		{
			Debug::Log(L"Create DepthStencil failed");
			Engine::Get()->Exit();
		}
		D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
		depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;
		D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
		depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
		depthOptimizedClearValue.DepthStencil.Stencil = 0;
		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_DEFAULT;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    props.CreationNodeMask = 1;
    props.VisibleNodeMask = 1;
		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Alignment = 0;
    desc.Width = width;
    desc.Height = height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 0;
    desc.Format = DXGI_FORMAT_D32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		RenderDX12::Get()->_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthOptimizedClearValue, IID_PPV_ARGS(&_dsBuffer));
		_dsDescriptor->SetName(L"Depth/Stencil Resource Heap");

		RenderDX12::Get()->_device->CreateDepthStencilView(_dsBuffer, &depthStencilDesc, _dsDescriptor->GetCPUDescriptorHandleForHeapStart());
	}
	RenderDX12::RenderDX12()
	{
		_this = this;
		_frameBufferCount = 2;
		_frameIndex = 0;
		_rtvDescriptorSize = 0;
		_device = nullptr;
		_debug = nullptr;
		_infoQueue = nullptr;
		_factory = nullptr;
		_rtvDescriptorHeap = nullptr;
	}
	RenderDX12::~RenderDX12()
	{
		Clear();
	}
	void RenderDX12::MemcpySubresource(_In_ const D3D12_MEMCPY_DEST* pDest, _In_ const D3D12_SUBRESOURCE_DATA* pSrc, SIZE_T RowSizeInBytes, UINT NumRows, UINT NumSlices)
	{
		for (UINT z = 0; z < NumSlices; ++z)
		{
			auto pDestSlice = reinterpret_cast<BYTE*>(pDest->pData) + pDest->SlicePitch*z;
			auto pSrcSlice = reinterpret_cast<const BYTE*>(pSrc->pData) + pSrc->SlicePitch*LONG_PTR(z);
			for(UINT y = 0; y < NumRows; ++y)
				memcpy(pDestSlice + pDest->RowPitch*y, pSrcSlice + pSrc->RowPitch*LONG_PTR(y), RowSizeInBytes);
		}
	}
	UINT64 RenderDX12::UpdateSubresources(_In_ ID3D12Resource* pDestinationResource, _In_ ID3D12Resource* pIntermediate, _In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource, _In_range_(0, D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources, UINT64 RequiredSize, _In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts, _In_reads_(NumSubresources) const UINT* pNumRows, _In_reads_(NumSubresources) const UINT64* pRowSizesInBytes, _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData)
	{
		// Minor validation
		auto IntermediateDesc = pIntermediate->GetDesc();
		auto DestinationDesc = pDestinationResource->GetDesc();
		if(IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER || IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset || RequiredSize > SIZE_T(-1) || (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER && (FirstSubresource != 0 || NumSubresources != 1)))
		{
			return 0;
		}
		BYTE* pData;
		HRESULT hr = pIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
		if(FAILED(hr))
		{
			return 0;
		}
		for(UINT i = 0; i < NumSubresources; ++i)
		{
			if (pRowSizesInBytes[i] > SIZE_T(-1))
				return 0;
			D3D12_MEMCPY_DEST DestData = { pData + pLayouts[i].Offset, pLayouts[i].Footprint.RowPitch, SIZE_T(pLayouts[i].Footprint.RowPitch) * SIZE_T(pNumRows[i]) };
			MemcpySubresource(&DestData, &pSrcData[i], static_cast<SIZE_T>(pRowSizesInBytes[i]), pNumRows[i], pLayouts[i].Footprint.Depth);
		}
		pIntermediate->Unmap(0, nullptr);
		if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		{
				_commandList->CopyBufferRegion(pDestinationResource, 0, pIntermediate, pLayouts[0].Offset, pLayouts[0].Footprint.Width);
		}
		else
		{
			for (UINT i = 0; i < NumSubresources; ++i)
			{
				D3D12_TEXTURE_COPY_LOCATION dst = {};
				dst.pResource = pDestinationResource;
				dst.SubresourceIndex = i + FirstSubresource;
				dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

				D3D12_TEXTURE_COPY_LOCATION src = {};
				src.pResource = pIntermediate;
				src.PlacedFootprint = pLayouts[i];
				src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				_commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
			}
		}
		return RequiredSize;
	}
	UINT64 RenderDX12::UpdateSubresources(_In_ ID3D12Resource* pDestinationResource, _In_ ID3D12Resource* pIntermediate, UINT64 IntermediateOffset, _In_range_(0,D3D12_REQ_SUBRESOURCES) UINT FirstSubresource, _In_range_(0,D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources, _In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData)
	{
			UINT64 RequiredSize = 0;
			UINT64 MemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64)) * NumSubresources;
			if (MemToAlloc > SIZE_MAX)
				return 0;
			void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
			if(pMem == nullptr)
				return 0;
			auto pLayouts = reinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
			UINT64* pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + NumSubresources);
			UINT* pNumRows = reinterpret_cast<UINT*>(pRowSizesInBytes + NumSubresources);
    
			auto Desc = pDestinationResource->GetDesc();
			ID3D12Device* pDevice = nullptr;
			pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
			pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
			pDevice->Release();
    
			UINT64 Result = UpdateSubresources(pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, pSrcData);
			HeapFree(GetProcessHeap(), 0, pMem);
			return Result;
	}
	void RenderDX12::BarrierTransition(_In_ ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter, UINT subresource, D3D12_RESOURCE_BARRIER_FLAGS flags)
	{
		_barrier = {};
    _barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    _barrier.Flags = flags;
    _barrier.Transition.pResource = pResource;
    _barrier.Transition.StateBefore =  stateBefore;
    _barrier.Transition.StateAfter = stateAfter;
    _barrier.Transition.Subresource = subresource;
		_commandList->ResourceBarrier(1, &_barrier);
	}
	void RenderDX12::ExecuteCommandList()
	{
		HRESULT hr = _commandList->Close();
		Debug::ThrowIfFailed(hr);
		ID3D12CommandList* ppCommandLists[] = { _commandList };
		_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	}
	void RenderDX12::ResetCommandList()
	{
		WaitForPreviousFrame();
		HRESULT hr = _commandAllocator[_frameIndex]->Reset();
		Debug::ThrowIfFailed(hr);
		hr = _commandList->Reset(_commandAllocator[_frameIndex], nullptr);
		Debug::ThrowIfFailed(hr);
	}
	void RenderDX12::Signal()
	{
		_fenceValue[_frameIndex]++;
		HRESULT hr = _commandQueue->Signal(_fence[_frameIndex], _fenceValue[_frameIndex]);
		Debug::ThrowIfFailed(hr);
	}
	void RenderDX12::WaitForPreviousFrame()
	{
		Signal();
		_frameIndex = _swapChain->GetCurrentBackBufferIndex();// swap the current rtv buffer index so we draw on the correct buffer

		// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
		if(_fence[_frameIndex]->GetCompletedValue() < _fenceValue[_frameIndex])
		{
			HRESULT hr = _fence[_frameIndex]->SetEventOnCompletion(_fenceValue[_frameIndex], _fenceEvent);// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
			Debug::ThrowIfFailed(hr);
			WaitForSingleObject(_fenceEvent, INFINITE);// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value has reached "fenceValue", we know the command queue has finished executing
		}
		_fenceValue[_frameIndex]++;// increment fenceValue for next frame
	}
	bool RenderDX12::Initialize(class Window *window, uint width, uint height)
	{
		if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&_debug))))
		{
			_debug->EnableDebugLayer();
		}
		UINT createFactoryFlags = 0;
		#if defined(_DEBUG)
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
		#endif
		HRESULT hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&_factory));
		Debug::ThrowIfFailed(hr);

		IDXGIAdapter1* adapter;
		DXGI_ADAPTER_DESC1 desc;
		int num = 0;
		bool adapterFound = false;
		while(_factory->EnumAdapters1(num, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			adapter->GetDesc1(&desc);
			if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				++num;
				adapter->Release();
				continue;
			}
			hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
			if(SUCCEEDED(hr))
			{
				adapterFound = true;
				break;
			}
			++num;
		}
		if(!adapterFound)
		{
			Debug::Log(L"Render initialization failed");
			return false;
		}
		hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
		adapter->Release();
		Debug::ThrowIfFailed(hr);

		#if defined(_DEBUG)
		if(_device->QueryInterface(IID_PPV_ARGS(&_infoQueue)))
		{
			_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
			// Suppress whole categories of messages
      //D3D12_MESSAGE_CATEGORY Categories[] = {};
			D3D12_MESSAGE_SEVERITY Severities[] =
      {
          D3D12_MESSAGE_SEVERITY_INFO
      };
 
      // Suppress individual messages by their ID
      D3D12_MESSAGE_ID DenyIds[] = {
          D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
          D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
          D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
      };
 
      D3D12_INFO_QUEUE_FILTER NewFilter = {};
      //NewFilter.DenyList.NumCategories = _countof(Categories);
      //NewFilter.DenyList.pCategoryList = Categories;
      NewFilter.DenyList.NumSeverities = _countof(Severities);
      NewFilter.DenyList.pSeverityList = Severities;
      NewFilter.DenyList.NumIDs = _countof(DenyIds);
      NewFilter.DenyList.pIDList = DenyIds;
 
      _infoQueue->PushStorageFilter(&NewFilter);
		}
		#endif

		D3D12_COMMAND_QUEUE_DESC cqDesc = {};
		hr = _device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&_commandQueue));
		Debug::ThrowIfFailed(hr);

		DXGI_MODE_DESC backBufferDesc = {};
		backBufferDesc.Width = width;
		backBufferDesc.Height = height;
		backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_SAMPLE_DESC sampleDesc = {};
		sampleDesc.Count = 1;
		_swapDesc = {};
		_swapDesc.BufferCount = _frameBufferCount;
		_swapDesc.BufferDesc = backBufferDesc;
		_swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		if(_frameBufferCount > 1)
			_swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		else
			_swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		_swapDesc.OutputWindow = window->GetHWND();
		_swapDesc.SampleDesc = sampleDesc;
		_swapDesc.Windowed = !window->IsFullscreen();
		IDXGISwapChain* tempSwap = nullptr;
		hr = _factory->CreateSwapChain(_commandQueue, &_swapDesc, &tempSwap);
		Debug::ThrowIfFailed(hr);

		_swapChain = static_cast<IDXGISwapChain3*>(tempSwap);
		_frameIndex = _swapChain->GetCurrentBackBufferIndex();

		_renderTargets.resize(_frameBufferCount);
		_commandAllocator.resize(_frameBufferCount);
		_fence.resize(_frameBufferCount);
		_fenceValue.resize(_frameBufferCount);
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = _frameBufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		hr = _device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvDescriptorHeap));
		Debug::ThrowIfFailed(hr);

		_rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		for (uint i = 0; i < _frameBufferCount; i++)
    {
      hr = _swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));// first we get the n'th buffer in the swap chain and store it in the n'th position of our ID3D12Resource array
      Debug::ThrowIfFailed(hr);

      _device->CreateRenderTargetView(_renderTargets[i], nullptr, rtvHandle);// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle

      // we increment the rtv handle by the rtv descriptor size we got above
			rtvHandle.ptr = SIZE_T(INT64(rtvHandle.ptr) + INT64(1)*INT64(_rtvDescriptorSize));
    }

		_depthStencil = new DepthStencil();
		_depthStencil->Create(width, height);

		for (uint i = 0; i < _frameBufferCount; i++)
		{
			hr = _device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator[i]));
			Debug::ThrowIfFailed(hr);
		}

		// create the command list with the first allocator
    hr = _device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator[0], NULL, IID_PPV_ARGS(&_commandList));
    Debug::ThrowIfFailed(hr);

		for(uint i = 0; i < _frameBufferCount; i++)
    {
      hr = _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence[i]));
      Debug::ThrowIfFailed(hr);
      _fenceValue[i] = 0;
    }
    _fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    Debug::ThrowIfFailed(hr);

		_viewport = RenderViewport();
		_viewport.SetWidth((float)width);
		_viewport.SetHeight((float)height);
		_scissor = Scissor();
		_scissor.SetRight(width);
		_scissor.SetBottom(height);

		Debug::Log(L"Render initialization is complete");

		return true;
	}
	void RenderDX12::Begin(const Color &clear, float depth)
	{
		BarrierTransition(_renderTargets[_frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		rtvHandle.ptr = SIZE_T(INT64(rtvHandle.ptr) + INT64(_frameIndex)*INT64(_rtvDescriptorSize));
		_commandList->OMSetRenderTargets(1, &rtvHandle, false, &_depthStencil->_dsDescriptor->GetCPUDescriptorHandleForHeapStart());
		_commandList->ClearRenderTargetView(rtvHandle, (float*)&clear, 0, nullptr);
		_commandList->ClearDepthStencilView(_depthStencil->_dsDescriptor->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		_commandList->RSSetViewports(1, (D3D12_VIEWPORT*)&_viewport); // set the viewports
		_commandList->RSSetScissorRects(1, (D3D12_RECT*)&_scissor); // set the scissor rects

	}
	void RenderDX12::Render(RenderMaterial *mat)
	{
		
	}
	void RenderDX12::Render(RenderMesh *mesh)
	{

	}
	void RenderDX12::End()
	{
		_barrier.Transition.StateBefore =  D3D12_RESOURCE_STATE_RENDER_TARGET;
    _barrier.Transition.StateAfter =  D3D12_RESOURCE_STATE_PRESENT;
		_commandList->ResourceBarrier(1, &_barrier);
		ExecuteCommandList();
		HRESULT hr = _swapChain->Present(0, 0);
		ResetCommandList();
		if(FAILED(hr))
    {
			Debug::Log(L"Begin Render failed");
			Engine::Get()->Exit();
    }
	}
	void RenderDX12::Clear()
	{
		WaitForPreviousFrame();
		CloseHandle(_fenceEvent);
		if(_depthStencil)
			delete _depthStencil;
		_depthStencil = nullptr;
		BOOL fs = false;
		if(_swapChain->GetFullscreenState(&fs, nullptr))
			_swapChain->SetFullscreenState(false, nullptr);
		for(ID3D12Fence* res: _fence)
			res->Release();
		if(_commandList)
			_commandList->Release();
		for(ID3D12CommandAllocator* res: _commandAllocator)
			res->Release();
		for(ID3D12Resource* res: _renderTargets)
			res->Release();
		if(_rtvDescriptorHeap)
			_rtvDescriptorHeap->Release();
		if(_infoQueue)
			_infoQueue->Release();
		if(_commandQueue)
			_commandQueue->Release();
		if(_debug)
			_debug->Release();
		if(_swapChain)
			_swapChain->Release();
		if(_device)
			_device->Release();
		if(_factory)
			_factory->Release();
	}
	void RenderDX12::SetViewport(const Viewport &port)
	{
		_viewport = port;
	}
	Viewport &RenderDX12::GetViewport()
	{
		return _viewport;
	}
	uint RenderDX12::GetWidth()
	{
		return _swapDesc.BufferDesc.Width;
	}
	uint RenderDX12::GetHeight()
	{
		return _swapDesc.BufferDesc.Height;
	}
	void RenderDX12::SetDefaultRenderTarget()
	{

	}
	void RenderDX12::Resize(uint width, uint height)
	{

	}

}