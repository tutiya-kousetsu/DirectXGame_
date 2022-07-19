#include "PostEffect.h"
#include "WinApp.h"
#include "Input.h"

#include <d3dx12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect()
	:Sprite(
		100,//テクスチャ番号
		{ 0, 0 },//座標
		{ 500.0f, 500.0f },//サイズ
		{ 1, 1, 1, 1 },//色
		{ 0.0f, 0.0f },//アンカーポイント
		false,//左右反転フラグ
		false)//上下反転フラグ
{
}

void PostEffect::Initialize()
{
	//HRESULT result;

	//Sprite::Initialize();

	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	DXGI_FORMAT_R8G8B8A8_UNORM,
	//	WinApp::window_width,
	//	(UINT)WinApp::window_height,
	//	1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	//);

	////テクスチャバッファの生成
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
	//		D3D12_MEMORY_POOL_L0),
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
	//	nullptr,
	//	IID_PPV_ARGS(&texBuff));
	//assert(SUCCEEDED(result));

	//{
	//	const UINT pixelCount = WinApp::window_width * WinApp::window_height;

	//	const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

	//	const UINT depthPitch = rowPitch * WinApp::window_height;

	//	UINT* img = new UINT[pixelCount];
	//	for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

	//	//テクスチャバッファにデータ転送
	//	result = texBuff->WriteToSubresource(0, nullptr,
	//		img, rowPitch, depthPitch);
	//	assert(SUCCEEDED(result));
	//	delete[] img;
	//}

	////SRV用デスクリプタヒープを生成
	//D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	//srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	//srvDescHeapDesc.NumDescriptors = 1;//テクスチャ枚数
	//result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));//生成
	//assert(SUCCEEDED(result));

	////SRV設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体

	//srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = 1;

	////デスクリプタヒープにSRV作成
	//device->CreateShaderResourceView(texBuff.Get(),//ビューと関連付ける
	//	&srvDesc,//テクスチャ設定情報
	//	descHeapSRV->GetCPUDescriptorHandleForHeapStart()//ヒープの先頭アドレス)
	//);

}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = XMMatrixIdentity();	// 行列の合成	
		this->constBuff->Unmap(0, nullptr);
	}

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));
	//cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	// ポリゴンの描画（4頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);

}
