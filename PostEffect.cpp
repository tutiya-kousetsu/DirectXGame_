#include "PostEffect.h"
#include "PipelineSet.h"
#include "Sprite.h"

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

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	spriteCommon->GetCommandList();

	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	// Z軸回転
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(this->rotation));
	// 平行移動
	this->matWorld *= XMMatrixTranslation(this->position.x, this->position.y, 0.0f);

	// 定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = this->matWorld * spriteCommon->GetMatProjection();
	constMap->color = this->color;
	this->constBuff->Unmap(0, nullptr);

	spriteCommon->PreDraw();


	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// ルートパラメータ0番に定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	// ルートパラメータ1番にシェーダリソースビューをセット
	spriteCommon->SetGraphicsRootDescriptorTable(1, this->texNumber);
	/*cmdList->SetPipelineState(pipelineSet->pipelinestate.Get());

	cmdList->SetGraphicsRootSignature(pipelineSet->rootsignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);*/

	// ポリゴンの描画（4頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);

}
