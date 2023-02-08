#pragma once
#include"Camera.h"
#include"PolygonDeferred.h"
#include"Cube.h"

using namespace DirectX;
using namespace Microsoft::WRL;


struct Vertex3DBuffer
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 diffuse;
	XMFLOAT2 texCoord;

};

struct ConstantBuffer
{
	XMFLOAT4X4 wvp;
	XMFLOAT4X4 wvpLight;
	XMFLOAT4X4 vp;
	XMFLOAT4X4 world;
	XMFLOAT4 reflectRate;
	XMFLOAT3 cameraPosition;
	float dammy;
};


class Renderer
{
private:
	struct Index {
		enum ResourceIndex {
			RESOURCE_INDEX_NORMAL,
			RESOURCE_INDEX_DIFFUSE,
			RESOURCE_INDEX_POSITION,
			RESOURCE_INDEX_DEPTH,
			RESOURCE_INDEX_REFLECT,
			RESOURCE_INDEX_GEOMETRY_MAX,
			RESOURCE_INDEX_SHADOW = RESOURCE_INDEX_GEOMETRY_MAX,
			RESOURCE_INDEX_MAX
		};


#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum, is3D)
#define PIPELINE_STATE_ID_TABLE\
		PIPELINE_STATE_ID(GEOMETRY, GeometryVS.cso, GeometryPS.cso, 5, true)\
		PIPELINE_STATE_ID(SHADOW,	 ShadowVS.cso,   ShadowPS.cso,  1, true)\
		PIPELINE_STATE_ID(LIGHTING, LightingVS.cso, LightingPS.cso, 1, true)\
		PIPELINE_STATE_ID(SPRITE,   SpriteVS.cso,   SpritePS.cso,   1, false)\

		enum PipelineStateID {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum, is3D) PIPELINE_STATE_ID_##name,
			PIPELINE_STATE_ID_TABLE
			PIPELINE_STATE_ID_MAX
		};



		static inline const char* GetVSFileName(PipelineStateID id) {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum, is3D)\
		case PIPELINE_STATE_ID_##name:\
				return #vsFileName;

			switch (id) {
				PIPELINE_STATE_ID_TABLE
			}
			return nullptr;
		}
		static inline const char* GetPSFileName(PipelineStateID id) {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum, is3D)\
		case PIPELINE_STATE_ID_##name:\
				return #psFileName;

			switch (id) {
				PIPELINE_STATE_ID_TABLE
			}
			return nullptr;
		}
		static const int GetResourceNum(PipelineStateID id) {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum, is3D)\
			case PIPELINE_STATE_ID_##name:\
			return resourceNum;

			switch (id) {
				PIPELINE_STATE_ID_TABLE
			}
			return -1;
		}
		static const int GetResourceMax() {
			int resourceMax = 0;
			for (int i = 0; i < Index::PIPELINE_STATE_ID_MAX; i++)
				resourceMax += Index::GetResourceNum(static_cast<Index::PipelineStateID>(i));

			return resourceMax;
		}
		static const bool GetTypeIs3D(PipelineStateID id) {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum, is3D)\
			case PIPELINE_STATE_ID_##name:\
				return is3D;

			switch (id){
				PIPELINE_STATE_ID_TABLE
			}

			return true;
		}
	};
private:

	static Renderer* m_instance;


	HWND								m_windowHandle;
	int									m_windowWidth;
	int									m_windowHeight;

	UINT64								m_frame;
	UINT								m_rtIndex;

	ComPtr<IDXGIFactory4>				m_factory;
	ComPtr<IDXGIAdapter3>				m_adapter;
	ComPtr<ID3D12Device>				m_device;
	ComPtr<ID3D12CommandQueue>			m_commandQueue;
	HANDLE								m_fenceEvent;
	ComPtr<ID3D12Fence>					m_fence;
	ComPtr<IDXGISwapChain3>				m_swapChain;
	ComPtr<ID3D12GraphicsCommandList>	m_graphicsCommandList;
	ComPtr<ID3D12CommandAllocator>		m_commandAllocator;
	ComPtr<ID3D12Resource>				m_renderTarget[2];
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			m_rtHandle[2];
	ComPtr<ID3D12Resource>				m_depthBuffer;
	ComPtr<ID3D12DescriptorHeap>		m_dsDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			m_dsHandle;

	ComPtr<ID3D12PipelineState>         m_pipelineStateGeometry;
	ComPtr<ID3D12PipelineState>         m_pipelineStateShadow;
	ComPtr<ID3D12PipelineState>         m_pipelineStateLightnig;
	ComPtr<ID3D12PipelineState>         m_pipelineStateSprite;
	ComPtr<ID3D12PipelineState>         m_pipelineState[Index::PIPELINE_STATE_ID_MAX];


	ComPtr<ID3D12RootSignature>         m_rootSignature;

	D3D12_RECT							m_scissorRect;
	D3D12_VIEWPORT						m_viewport;


	ComPtr<ID3D12Resource>				m_resource[Index::RESOURCE_INDEX_MAX];

	D3D12_CPU_DESCRIPTOR_HANDLE			m_rtHandleGeometry[Index::RESOURCE_INDEX_GEOMETRY_MAX];
	D3D12_CPU_DESCRIPTOR_HANDLE			m_rtHandleShadow;
	ComPtr<ID3D12DescriptorHeap>		m_rtvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>		m_srvDescriptorHeap;


	std::unique_ptr<PolygonDeferred>   m_polygonDeferred;
	
	void CreateDevice();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateRendererTarget();
	void CreateDepthStencilBuffer();
	void CreateResource();
	void CreateRTVDescriptorHeap();
	void CreateSRVDescriptorHeap();
	void CreateCommandList();
	void CreateRootSignature();
	void CreatePipelineState(Index::PipelineStateID id);


	void SetResourceBarrier(ID3D12Resource* Resource, D3D12_RESOURCE_STATES BeforeState, D3D12_RESOURCE_STATES AfterState);

public:

	Renderer();

	void GeometryPassStart();
	void GeometryPassEnd();
	void ShadowPassStart();
	void ShadowPassEnd();
	void Draw2DStart();
	void Draw2DEnd();
	void DrawEnd();
	static Renderer* GetInstance() { return m_instance; }
	ComPtr<ID3D12Device> GetDevice() { return m_device; }
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() { return m_graphicsCommandList; }
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_descriptorHeap; }
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return m_graphicsCommandList; }
	ComPtr<ID3D12DescriptorHeap> GetSRVDescriptorHeap() { return m_srvDescriptorHeap; }

#ifdef _DEBUG
private:
	ComPtr<ID3D12DescriptorHeap>		m_imguiSRVDescriptorHeap;
public:
	ComPtr<ID3D12DescriptorHeap> GetImguiSRVDescriptorHeap() { return m_imguiSRVDescriptorHeap; }
#endif // _DEBUG
};

