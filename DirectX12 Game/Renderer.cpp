#ifdef _DEBUG
#include"ImguiRenderer.h"
#endif // _DEBUG


#include "Main.h"
#include "Renderer.h"

#include"CameraManager.h"



Renderer* Renderer::m_instance = nullptr;

void Renderer::CreateDevice() {
	UINT flag{};
	CreateDXGIFactory2(flag, IID_PPV_ARGS(m_factory.GetAddressOf()));
	m_factory->EnumAdapters(0, (IDXGIAdapter**)m_adapter.GetAddressOf());
	D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&m_device));
}
void Renderer::CreateCommandQueue() {
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = 0;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	m_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(m_commandQueue.GetAddressOf()));
	m_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()));
}
void Renderer::CreateSwapChain() {
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	ComPtr<IDXGISwapChain> swapChain{};

	swapChainDesc.BufferDesc.Width = m_windowWidth;
	swapChainDesc.BufferDesc.Height = m_windowHeight;
	swapChainDesc.OutputWindow = m_windowHandle;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;


	m_factory->CreateSwapChain(m_commandQueue.Get(), &swapChainDesc, swapChain.GetAddressOf());
	swapChain.As(&m_swapChain);
	m_rtIndex = m_swapChain->GetCurrentBackBufferIndex();
}
void Renderer::CreateRendererTarget() {
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	heapDesc.NumDescriptors = 2;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;
	m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_descriptorHeap.GetAddressOf()));

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	UINT size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (UINT i = 0; i < 2; ++i)
	{
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTarget[i]));

		m_rtHandle[i] = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		m_rtHandle[i].ptr += size * i;
		m_device->CreateRenderTargetView(m_renderTarget[i].Get(), nullptr, m_rtHandle[i]);
	}
}
void Renderer::CreateDepthStencilBuffer() {
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.NodeMask = 0;
	m_device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_dsDescriptorHeap));



	D3D12_HEAP_PROPERTIES heapProperties{};
	D3D12_RESOURCE_DESC resourceDesc{};
	D3D12_CLEAR_VALUE clearValue{};

	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = m_windowWidth;
	resourceDesc.Height = m_windowHeight;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 0;
	resourceDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&m_depthBuffer));



	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	m_dsHandle = m_dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	m_device->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, m_dsHandle);
}
void Renderer::CreateResource() {
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = m_windowWidth;
	resourceDesc.Height = m_windowHeight;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;



	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 1.0f;
	clearValue.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	for (int i = 0; i < Index::RTV_RESOURCE_INDEX_MAX; i++) {
		m_device->CreateCommittedResource(&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(&m_resource[i]));
	}


}
void Renderer::CreateRTVDescriptorHeap() {
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NumDescriptors = Index::RTV_RESOURCE_INDEX_MAX;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.NodeMask = 0;

	m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvDescriptorHeap));



	//RTV
	{
		unsigned int size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();


		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.Texture2D.PlaneSlice = 0;

		for (int i = 0; i < Index::RTV_RESOURCE_INDEX_GEOMETRY_MAX; i++) {
			m_device->CreateRenderTargetView(m_resource[i].Get(), &rtvDesc, handle);
			m_rtHandleGeometry[i] = handle;
			handle.ptr += size;
		}

		m_device->CreateRenderTargetView(m_resource[Index::RTV_RESOURCE_INDEX_SHADOW].Get(), &rtvDesc, handle);
		m_rtHandleShadow = handle;
		handle.ptr += size;

		m_device->CreateRenderTargetView(m_resource[Index::RTV_RESOURCE_INDEX_ALPHA].Get(), &rtvDesc, handle);
		m_rtHandleGeometryAlpha = handle;
		handle.ptr += size;
	}

}
void Renderer::CreateSRVDescriptorHeap() {

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = Index::RTV_RESOURCE_INDEX_MAX;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NodeMask = 0;

	m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_srvDescriptorHeap));

	//SRV
	unsigned int size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.PlaneSlice = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	

	for (int i = 0; i < Index::RTV_RESOURCE_INDEX_GEOMETRY_MAX; i++) {
		m_device->CreateShaderResourceView(m_resource[i].Get(), &srvDesc, handle);
		handle.ptr += (size);
	}

	m_device->CreateShaderResourceView(m_resource[Index::RTV_RESOURCE_INDEX_SHADOW].Get(), &srvDesc, handle);
	handle.ptr += (size);

	m_device->CreateShaderResourceView(m_resource[Index::RTV_RESOURCE_INDEX_ALPHA].Get(), &srvDesc, handle);
	handle.ptr += (size);

	ptrSize = handle.ptr;
}
void Renderer::CreateCommandList() {
	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_graphicsCommandList));
}
void Renderer::CreateRootSignature() {
	D3D12_ROOT_PARAMETER rootParameters[2]{};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[0].Descriptor.RegisterSpace = 0;

	D3D12_DESCRIPTOR_RANGE range[1]{};
	range[0].NumDescriptors = Index::RTV_RESOURCE_INDEX_MAX+ Index::MODEL_TEX_RESOURCE_NUM;
	range[0].BaseShaderRegister = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[1].DescriptorTable.pDescriptorRanges = &range[0];

	//サンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.ShaderRegister = 0;
	samplerDesc.RegisterSpace = 0;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC rootSignature_desc{};
	rootSignature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignature_desc.NumParameters = _countof(rootParameters);
	rootSignature_desc.pParameters = rootParameters;
	rootSignature_desc.NumStaticSamplers = 1;
	rootSignature_desc.pStaticSamplers = &samplerDesc;

	ComPtr<ID3DBlob> blob{};
	D3D12SerializeRootSignature(&rootSignature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, nullptr);
	m_device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
}
void Renderer::CreatePipelineState(Index::PipelineStateID id) {

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};

	std::vector<char> vertexShader;
	std::vector<char> pixelShader;

	//頂点シェーダー読み込み
	{
		std::ifstream file(Index::GetVSFileName(id), std::ios_base::in | std::ios_base::binary);
		assert(file);

		file.seekg(0, std::ios_base::end);
		int filesize = (int)file.tellg();
		file.seekg(0, std::ios_base::beg);

		vertexShader.resize(filesize);
		file.read(&vertexShader[0], filesize);

		file.close();

		pipelineStateDesc.VS.pShaderBytecode = &vertexShader[0];
		pipelineStateDesc.VS.BytecodeLength = filesize;
	}


	//ピクセルシェーダー
	{
		std::ifstream file(Index::GetPSFileName(id), std::ios_base::in | std::ios_base::binary);
		assert(file);

		file.seekg(0, std::ios_base::end);
		int filesize = (int)file.tellg();
		file.seekg(0, std::ios_base::beg);

		pixelShader.resize(filesize);
		file.read(&pixelShader[0], filesize);
		file.close();

		pipelineStateDesc.PS.pShaderBytecode = &pixelShader[0];
		pipelineStateDesc.PS.BytecodeLength = filesize;
	}

	//インプットレイアウト
	D3D12_INPUT_ELEMENT_DESC InputElementDesc[] =
	{
		{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,      D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 4 * 3,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 4 * 6,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 4 * 9,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 12,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, 4 * 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }

	};
	pipelineStateDesc.InputLayout.pInputElementDescs = InputElementDesc;
	pipelineStateDesc.InputLayout.NumElements = _countof(InputElementDesc);

	pipelineStateDesc.SampleDesc.Count = 1;
	pipelineStateDesc.SampleDesc.Quality = 0;
	pipelineStateDesc.SampleMask = UINT_MAX;

	pipelineStateDesc.NumRenderTargets = Index::GetResourceNum(id);
	for (int i = 0; i < static_cast<int>(pipelineStateDesc.NumRenderTargets); i++) {
		pipelineStateDesc.RTVFormats[i] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	}

	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineStateDesc.pRootSignature = m_rootSignature.Get();

	//ラスタライザステート
	pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	pipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineStateDesc.RasterizerState.FrontCounterClockwise = FALSE;
	pipelineStateDesc.RasterizerState.DepthBias = 0;
	pipelineStateDesc.RasterizerState.DepthBiasClamp = 0;
	pipelineStateDesc.RasterizerState.SlopeScaledDepthBias = 0;
	pipelineStateDesc.RasterizerState.DepthClipEnable = TRUE;
	pipelineStateDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	pipelineStateDesc.RasterizerState.AntialiasedLineEnable = FALSE;
	pipelineStateDesc.RasterizerState.MultisampleEnable = FALSE;

	//ブレンドステート
	for (int i = 0; i < _countof(pipelineStateDesc.BlendState.RenderTarget); ++i)
	{
		std::string typeName = Index::GetName(id);
		if (typeName == "GEOMETRY_ALPHA") {
			pipelineStateDesc.BlendState.RenderTarget[i].BlendEnable = TRUE;
			pipelineStateDesc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			pipelineStateDesc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			pipelineStateDesc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			pipelineStateDesc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
			pipelineStateDesc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			pipelineStateDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			pipelineStateDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			pipelineStateDesc.BlendState.RenderTarget[i].LogicOpEnable = FALSE;
			pipelineStateDesc.BlendState.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;

		}
		else if (typeName == "SPRITE") {
			pipelineStateDesc.BlendState.RenderTarget[i].BlendEnable = TRUE;
			pipelineStateDesc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			pipelineStateDesc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			pipelineStateDesc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			pipelineStateDesc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
			pipelineStateDesc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
			pipelineStateDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			pipelineStateDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			pipelineStateDesc.BlendState.RenderTarget[i].LogicOpEnable = FALSE;
			pipelineStateDesc.BlendState.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_CLEAR;
		}
		else  {
			pipelineStateDesc.BlendState.RenderTarget[i].BlendEnable = FALSE;
			pipelineStateDesc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
			pipelineStateDesc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
			pipelineStateDesc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			pipelineStateDesc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
			pipelineStateDesc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
			pipelineStateDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			pipelineStateDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			pipelineStateDesc.BlendState.RenderTarget[i].LogicOpEnable = FALSE;
			pipelineStateDesc.BlendState.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_CLEAR;
		}
	}
	pipelineStateDesc.BlendState.AlphaToCoverageEnable = FALSE;
	pipelineStateDesc.BlendState.IndependentBlendEnable = FALSE;

	//デブス・ステンシルステート
	pipelineStateDesc.DepthStencilState.DepthEnable = TRUE;
	pipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	pipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	pipelineStateDesc.DepthStencilState.StencilEnable = FALSE;
	pipelineStateDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	pipelineStateDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	pipelineStateDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pipelineStateDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;


	m_device->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&m_pipelineState[id]));
}
void Renderer::SetResourceBarrier(ID3D12Resource* Resource, D3D12_RESOURCE_STATES BeforeState, D3D12_RESOURCE_STATES AfterState) {
	D3D12_RESOURCE_BARRIER resourceBarrier{};

	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = Resource;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resourceBarrier.Transition.StateBefore = BeforeState;
	resourceBarrier.Transition.StateAfter = AfterState;

	m_graphicsCommandList->ResourceBarrier(1, &resourceBarrier);
}



Renderer::Renderer() {
	m_instance = this;


	m_windowHandle = GetWind();
	m_windowWidth = SCREEN_WIDTH;
	m_windowHeight = SCREEN_HEIGHT;
	m_frame = 0;
	m_rtIndex = 0;



	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = (FLOAT)m_windowWidth;
	m_viewport.Height = (FLOAT)m_windowHeight;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;

	m_scissorRect.top = 0;
	m_scissorRect.left = 0;
	m_scissorRect.right = m_windowWidth;
	m_scissorRect.bottom = m_windowHeight;



	//デバイス生成
	CreateDevice();

	//コマンドキュー生成
	CreateCommandQueue();

	//スワップチェーン生成
	CreateSwapChain();

	//レンダーターゲット生成
	CreateRendererTarget();

	//デプス・ステンシルバッファ生成
	CreateDepthStencilBuffer();

	//リソース生成
	CreateResource();

	//RTVデスクリプタヒープ
	CreateRTVDescriptorHeap();

	//SRVデスクリプタヒープ
	CreateSRVDescriptorHeap();

	//コマンドリスト生成
	CreateCommandList();

	//ルートシグネチャ生成
	CreateRootSignature();

	//ジパイプライン
	for (int i = 0; i < Index::PIPELINE_STATE_ID_MAX; i++)
		CreatePipelineState(static_cast<Index::PipelineStateID>(i));

#ifdef _DEBUG
	//Imgui
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		HRESULT hr = m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_imguiSRVDescriptorHeap));
		assert(SUCCEEDED(hr));
	}
#endif // _DEBUG

	m_polygonDeferred = std::make_unique<PolygonDeferred>();
}


void Renderer::GeometryPassStart() {
	FLOAT clearColor[4] = { 0, 0, 0, 1.0f };
	m_nowBasePipelineStateID = Index::PIPELINE_STATE_ID_GEOMETRY;

	//レンダーターゲット用リソースバリア
	for (int i = 0; i < Index::RTV_RESOURCE_INDEX_GEOMETRY_MAX; i++) {
		SetResourceBarrier(m_resource[i].Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

	//デプスバッファ・レンダーターゲットのクリア
	m_graphicsCommandList->ClearDepthStencilView(m_dsHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	for (int i = 0; i < Index::RTV_RESOURCE_INDEX_GEOMETRY_MAX; i++) {
		m_graphicsCommandList->ClearRenderTargetView(m_rtHandleGeometry[i], clearColor, 0, nullptr);
	}


	//ルートシグネチャとパイプラインステートジオメトリの設定
	m_graphicsCommandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_graphicsCommandList->SetPipelineState(m_pipelineState[Index::PIPELINE_STATE_ID_GEOMETRY].Get());


	//ビューポートとシザー矩形の設定
	m_graphicsCommandList->RSSetViewports(1, &m_viewport);
	m_graphicsCommandList->RSSetScissorRects(1, &m_scissorRect);

	//レンダーターゲットの設定
	m_graphicsCommandList->OMSetRenderTargets(Index::RTV_RESOURCE_INDEX_GEOMETRY_MAX, m_rtHandleGeometry, TRUE, &m_dsHandle);
}
void Renderer::GeometryPassEnd() {
	//シェーダーリソース用リソースバリア
	for (int i = 0; i < Index::RTV_RESOURCE_INDEX_GEOMETRY_MAX; i++) {
		SetResourceBarrier(m_resource[i].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	}
}

void Renderer::GeometryAlphaPassStart() {
	FLOAT clearColor[4] = { 0, 0, 0, 1.0f };
	m_nowBasePipelineStateID = Index::PIPELINE_STATE_ID_GEOMETRY_ALPHA;

	//レンダーターゲット用リソースバリア
	SetResourceBarrier(m_resource[Index::RTV_RESOURCE_INDEX_ALPHA].Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//デプスバッファ・レンダーターゲットのクリア
	m_graphicsCommandList->ClearDepthStencilView(m_dsHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	m_graphicsCommandList->ClearRenderTargetView(m_rtHandleGeometryAlpha, clearColor, 0, nullptr);


	//ルートシグネチャとパイプラインステートジオメトリの設定
	m_graphicsCommandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_graphicsCommandList->SetPipelineState(m_pipelineState[Index::PIPELINE_STATE_ID_GEOMETRY_ALPHA].Get());


	//ビューポートとシザー矩形の設定
	m_graphicsCommandList->RSSetViewports(1, &m_viewport);
	m_graphicsCommandList->RSSetScissorRects(1, &m_scissorRect);

	//レンダーターゲットの設定
	m_graphicsCommandList->OMSetRenderTargets(1, &m_rtHandleGeometryAlpha, TRUE, &m_dsHandle);
}
void Renderer::GeometryAlphaPassEnd() {
	SetResourceBarrier(m_resource[Index::RTV_RESOURCE_INDEX_ALPHA].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}

void Renderer::ShadowPassStart() {
	FLOAT clearColor[4] = { 0, 0, 0, 1.0f };
	m_nowBasePipelineStateID = Index::PIPELINE_STATE_ID_SHADOW;

	//レンダーターゲット用リソースバリア
	SetResourceBarrier(m_resource[Index::RTV_RESOURCE_INDEX_SHADOW].Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//デプスバッファ・レンダーターゲットのクリア
	m_graphicsCommandList->ClearDepthStencilView(m_dsHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	m_graphicsCommandList->ClearRenderTargetView(m_rtHandleShadow, clearColor, 0, nullptr);


	//ルートシグネチャとパイプラインステートジオメトリの設定
	m_graphicsCommandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_graphicsCommandList->SetPipelineState(m_pipelineState[Index::PIPELINE_STATE_ID_SHADOW].Get());


	//ビューポートとシザー矩形の設定
	m_graphicsCommandList->RSSetViewports(1, &m_viewport);
	m_graphicsCommandList->RSSetScissorRects(1, &m_scissorRect);

	//レンダーターゲットの設定
	m_graphicsCommandList->OMSetRenderTargets(1, &m_rtHandleShadow, TRUE, &m_dsHandle);
}
void Renderer::ShadowPassEnd() {
	SetResourceBarrier(m_resource[Index::RTV_RESOURCE_INDEX_SHADOW].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}


void Renderer::Draw2DStart() {

	FLOAT clearColor[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	m_nowBasePipelineStateID = Index::PIPELINE_STATE_ID_SPRITE;

	//レンダーたゲット用リソースバリア
	SetResourceBarrier(m_renderTarget[m_rtIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//デプスパッファ・レンダーターゲットのクリア
	m_graphicsCommandList->ClearDepthStencilView(m_dsHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	m_graphicsCommandList->ClearRenderTargetView(m_rtHandle[m_rtIndex], clearColor, 0, nullptr);

	//パイプラインステートライティング
	m_graphicsCommandList->SetPipelineState(m_pipelineState[Index::PIPELINE_STATE_ID_LIGHTING].Get());

	//レンダーターゲットの設定
	m_graphicsCommandList->OMSetRenderTargets(1, &m_rtHandle[m_rtIndex], TRUE, &m_dsHandle);

	//デファードレンダリング2Dポリゴンの描画
	m_polygonDeferred.get()->Draw(m_srvDescriptorHeap.Get());

	//パイプラインステートライティング
	m_graphicsCommandList->SetPipelineState(m_pipelineState[Index::PIPELINE_STATE_ID_SPRITE].Get());
}
void Renderer::Draw2DEnd() {

#ifdef _DEBUG
	//Imgui
	{
		ID3D12DescriptorHeap* dh[] = { m_imguiSRVDescriptorHeap.Get() };
		m_graphicsCommandList->SetDescriptorHeaps(1, dh);
		ImguiRenderer::GetInstance()->Draw();
	}
#endif // _DEBUG

	//プレゼント用リソースバリア
	SetResourceBarrier(m_renderTarget[m_rtIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

}

void Renderer::DrawEnd() {

	m_graphicsCommandList->Close();


	//コマンドリスト実行
	ID3D12CommandList* const command_lists = m_graphicsCommandList.Get();
	m_commandQueue->ExecuteCommandLists(1, &command_lists);



	//実行したコマンドの終了待ち
	const UINT64 fence = m_frame;
	m_commandQueue->Signal(m_fence.Get(), fence);
	m_frame++;

	if (m_fence->GetCompletedValue() < fence)
	{
		m_fence->SetEventOnCompletion(fence, m_fenceEvent);

		WaitForSingleObject(m_fenceEvent, INFINITE);
	}


	m_commandAllocator->Reset();
	m_graphicsCommandList->Reset(m_commandAllocator.Get(), nullptr);
	m_swapChain->Present(1, 0);

	//カレントのバックバッファのインデックスを取得する
	m_rtIndex = m_swapChain->GetCurrentBackBufferIndex();
}