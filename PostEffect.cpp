#include "PostEffect.h"

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

void PostEffect::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	if (this->isInvisible) {
		return;
	}

	ID3D12GraphicsCommandList* cmdList = spriteCommon->GetCommandList();

	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// ルートパラメータ0番に定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	// ルートパラメータ1番にシェーダリソースビューをセット
	spriteCommon->SetGraphicsRootDescriptorTable(1, this->texNumber);

	// ポリゴンの描画（4頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);
}