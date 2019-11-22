#pragma once
#include "../Render.h"
#include "../../Math/Color.h"



namespace Led
{
	class RenderViewport: private D3D12_VIEWPORT, public Viewport
	{
	public: 
		RenderViewport()
		{
			TopLeftX = TopLeftY = MinDepth = 0.0f;
			Width = Height = MaxDepth = 1.0f;
		}
		RenderViewport(const Viewport &v)
		{
			TopLeftX = TopLeftY = MinDepth = 0.0f;
			Width = Height = MaxDepth = 1.0f;
		}
		void SetX(float value) { TopLeftX = value; };
		void SetY(float value) { TopLeftY = value; };
		void SetWidth(float value) { Width = value; };
		void SetHeight(float value) { Height = value; };
		void SetMinDepth(float value) { MinDepth = value; };
		void SetMaxDepth(float value) { MaxDepth = value; };
		float GetX() { return TopLeftX; };
		float GetY() { return TopLeftY; };
		float GetWidth() { return Width; };
		float GetHeight() { return Height; };
		float GetMinDepth() { return MinDepth; };
		float GetMaxDepth() { return MaxDepth; };
	};
	class DepthStencil
	{
		friend class RenderDX12;
	private:
		ID3D12Resource* _dsBuffer;
		ID3D12DescriptorHeap* _dsDescriptor;
	public:
		DepthStencil();
		~DepthStencil();
		void Create(uint width, uint height);
	};
	class Scissor: private D3D12_RECT
	{
	public: 
		Scissor()
		{
			left = top = 0;
			right = bottom = 1;
		}
		void SetLeft(long value) { left = value; };
		void SetTop(long value) { top = value; };
		void SetRight(long value) { right = value; };
		void SetBottom(long value) { bottom = value; };
		long GetLeft() { return left; };
		long GetTop() { return top; };
		long GetRight() { return right; };
		long GetBottom() { return bottom; };
	};
	class RenderDX12: public Render
	{
		friend class RenderMaterial;
		friend class RenderMesh;
		friend class DepthStencil;
		friend class ConstantBuffer;
		friend class RenderTexture;
	private:
		uint _frameBufferCount;
		uint _frameIndex;
		static RenderDX12* _this;
		ID3D12Device* _device;
		IDXGISwapChain3* _swapChain;
		ID3D12CommandQueue* _commandQueue;
		ID3D12Debug* _debug;
		ID3D12InfoQueue *_infoQueue;
		IDXGIFactory4* _factory;
		ID3D12DescriptorHeap* _rtvDescriptorHeap;
		vector<ID3D12Resource*> _renderTargets;
		vector<ID3D12CommandAllocator*> _commandAllocator;
		ID3D12GraphicsCommandList* _commandList;
		vector<ID3D12Fence*> _fence;    // an object that is locked while our command list is being executed by the gpu. We need as many as we have allocators (more if we want to know when the gpu is finished with an asset)
		HANDLE _fenceEvent; // a handle to an event when our fence is unlocked by the gpu
		vector<UINT64> _fenceValue; // this value is incremented each frame. each fence will have its own value
		int _rtvDescriptorSize;
		void Signal();
		void WaitForPreviousFrame();
		D3D12_RESOURCE_BARRIER _barrier;
		DXGI_SWAP_CHAIN_DESC _swapDesc;
		void MemcpySubresource(_In_ const D3D12_MEMCPY_DEST* pDest, _In_ const D3D12_SUBRESOURCE_DATA* pSrc, SIZE_T RowSizeInBytes, UINT NumRows, UINT NumSlices);
		UINT64 UpdateSubresources(_In_ ID3D12Resource* pDestinationResource, _In_ ID3D12Resource* pIntermediate, _In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource, _In_range_(0, D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources, UINT64 RequiredSize, _In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts, _In_reads_(NumSubresources) const UINT* pNumRows, _In_reads_(NumSubresources) const UINT64* pRowSizesInBytes, _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData);
		UINT64 UpdateSubresources(_In_ ID3D12Resource* pDestinationResource, _In_ ID3D12Resource* pIntermediate, UINT64 IntermediateOffset, _In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource, _In_range_(0, D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources, _In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData);
		void BarrierTransition(_In_ ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter, UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);
		void ExecuteCommandList();
		void ResetCommandList();
		inline uint GetFrameBufferCount() { return _frameBufferCount; }
		inline uint GetFrameIndex() { return _frameIndex; }

		RenderViewport _viewport;
		Scissor _scissor;
		DepthStencil *_depthStencil;
	public:
		static RenderDX12 *Get() {return _this;};
		RenderDX12();
		~RenderDX12();
		bool Initialize(class Window *window, uint width = 1024, uint height = 768);
		void Begin(const struct Color &clear, float depth);
		void End();
		void Clear();
		void SetViewport(const Viewport &port);
		Viewport &GetViewport();
		uint GetWidth();
		uint GetHeight();
		void SetDefaultRenderTarget();
		void Resize(uint width, uint height);
		void Render(RenderMaterial *mat);
		void Render(RenderMesh *mesh);
	};
}