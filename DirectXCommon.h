#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <wrl.h>

#include"WinApp.h"


class DirectXCommon
{
public:
	void Initialize(WinApp* winApp);

	void PreDraw();
	void PostDraw();

	ID3D12Device* GetDev() { return dev.Get(); }

	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

private:
	void InitializeDevice();

	void InitializeCommand();

	void InitializeSwapchain();

	void InitializeRecderTargetView();

	void InitializeDepthBuffer();

	void InitializeFence();

	//WindowsAPI
	WinApp* winApp = nullptr;

	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	Microsoft::WRL::ComPtr<ID3D12Fence> fence;

	UINT64 fenceVal = 0;

};

