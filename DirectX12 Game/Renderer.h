#pragma once
#include"CameraManager.h"
#include"PolygonDeferred.h"
#include"Cube.h"
#include<string>
#include <memory>

using namespace DirectX;
using namespace Microsoft::WRL;


struct Vertex3DBuffer
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
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
	bool Dammy=false;
	XMFLOAT4 waterParam;
};


class Renderer
{
public:
	struct Index {
		enum MODEL_TEX_RESOURCE_NUM {
			MODEL_TEX_RESOURCE_NUM = 5
		};
		enum RtvResourceIndex {
			RTV_RESOURCE_INDEX_NORMAL,
			RTV_RESOURCE_INDEX_DIFFUSE,
			RTV_RESOURCE_INDEX_POSITION,
			RTV_RESOURCE_INDEX_DEPTH,
			RTV_RESOURCE_INDEX_REFLECT,
			RTV_RESOURCE_INDEX_GEOMETRY_MAX,
			RTV_RESOURCE_INDEX_SHADOW = RTV_RESOURCE_INDEX_GEOMETRY_MAX,//GEOMETRY_MAXは最大値取得用なので1つずらす感じ
			RTV_RESOURCE_INDEX_ALPHA,

			RTV_RESOURCE_INDEX_MAX
		};


		//ここを増やすとパイプラインステートが増える
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum)
#define PIPELINE_STATE_ID_TABLE\
		PIPELINE_STATE_ID(GEOMETRY,			GeometryVS.cso,		 GeometryPS.cso,		RTV_RESOURCE_INDEX_GEOMETRY_MAX)\
		PIPELINE_STATE_ID(GEOMETRY_ALPHA,	GeometryAlphaVS.cso, GeometryAlphaPS.cso,	1)\
		PIPELINE_STATE_ID(WATER,			WaterVS.cso,		 WaterPS.cso,			1)\
		PIPELINE_STATE_ID(SKYDOME,			SkydomeVS.cso,		 SkydomePS.cso,			1)\
		PIPELINE_STATE_ID(SHADOW,			ShadowVS.cso,		 ShadowPS.cso,			1)\
		PIPELINE_STATE_ID(LIGHTING,			LightingVS.cso,		 LightingPS.cso,		1)\
		PIPELINE_STATE_ID(SPRITE,			SpriteVS.cso,		 SpritePS.cso,			1)\

		enum PipelineStateID {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum) PIPELINE_STATE_ID_##name,
			PIPELINE_STATE_ID_TABLE
			PIPELINE_STATE_ID_MAX
		};


		static inline const char* GetVSFileName(PipelineStateID id) {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum)\
		case PIPELINE_STATE_ID_##name:\
				return #vsFileName;

			switch (id) {
				PIPELINE_STATE_ID_TABLE
			}
			return nullptr;
		}
		static inline const char* GetPSFileName(PipelineStateID id) {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum)\
		case PIPELINE_STATE_ID_##name:\
				return #psFileName;

			switch (id) {
				PIPELINE_STATE_ID_TABLE
			}
			return nullptr;
		}
		static const int GetResourceNum(PipelineStateID id) {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum)\
			case PIPELINE_STATE_ID_##name:\
			return resourceNum;

			switch (id) {
				PIPELINE_STATE_ID_TABLE
			}
			return -1;
		}
		static const int GetAllResourceMax() {
			int resourceMax = 0;
			for (int i = 0; i < Index::PIPELINE_STATE_ID_MAX; i++)
				resourceMax += Index::GetResourceNum(static_cast<Index::PipelineStateID>(i));

			return resourceMax;
		}
		static inline const std::string GetName(PipelineStateID id) {
#undef PIPELINE_STATE_ID
#define PIPELINE_STATE_ID(name, vsFileName, psFileName, resourceNum)\
		case PIPELINE_STATE_ID_##name:\
				return #name;

			switch (id) {
				PIPELINE_STATE_ID_TABLE
			}
			return "";
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


	ComPtr<ID3D12Resource>				m_resource[Index::RTV_RESOURCE_INDEX_MAX];

	D3D12_CPU_DESCRIPTOR_HANDLE			m_rtHandleGeometry[Index::RTV_RESOURCE_INDEX_GEOMETRY_MAX];
	D3D12_CPU_DESCRIPTOR_HANDLE			m_rtHandleShadow;
	D3D12_CPU_DESCRIPTOR_HANDLE			m_rtHandleGeometryAlpha;


	ComPtr<ID3D12DescriptorHeap>		m_rtvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>		m_srvDescriptorHeap;

	Index::PipelineStateID m_nowBasePipelineStateID = Index::PipelineStateID::PIPELINE_STATE_ID_GEOMETRY;
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
	SIZE_T ptrSize = 0;
	Renderer();

	void GeometryPassStart();
	void GeometryPassEnd();
	void GeometryAlphaPassStart();
	void GeometryAlphaPassEnd();
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
	ComPtr<ID3D12Resource>GetResource(Index::RtvResourceIndex index) { return m_resource[index]; }
	void CreateConstantBuffer(ComPtr<ID3D12Resource> & constantBuffer) {
		ComPtr<ID3D12Device> device = Renderer::GetInstance()->GetDevice();
		D3D12_HEAP_PROPERTIES heapProperties{};
		D3D12_RESOURCE_DESC   resourceDesc{};

		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 0;
		heapProperties.VisibleNodeMask = 0;

		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;

		//定数バッファの作成
		resourceDesc.Width = 256;
		device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
			&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constantBuffer));
	}

	void SetNowBasePipeline() {
		m_graphicsCommandList->SetPipelineState(m_pipelineState[m_nowBasePipelineStateID].Get());
	}
	void SetPipeline(Index::PipelineStateID pipelineID){
		m_graphicsCommandList->SetPipelineState(m_pipelineState[pipelineID].Get());
	}
	Index::PipelineStateID GetNowPipelineStateID() { return m_nowBasePipelineStateID; }

#ifdef _DEBUG
private:
	ComPtr<ID3D12DescriptorHeap>		m_imguiSRVDescriptorHeap;
public:
	ComPtr<ID3D12DescriptorHeap> GetImguiSRVDescriptorHeap() { return m_imguiSRVDescriptorHeap; }
#endif // _DEBUG
};

