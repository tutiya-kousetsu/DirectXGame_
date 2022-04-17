#include "Sprite.h"
#include <d3dx12.h>

using namespace DirectX;

Sprite* Sprite::Create(UINT texNumber, DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	//メモリ確保
	Sprite* instance = new Sprite;
	//インスタンス初期化
	instance->Initialize(texNumber, anchorpoint, isFlipX, isFlipY);
	
	return instance;
}

void Sprite::Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;

	//メンバ変数に書き込む
	this->texNumber = texNumber;
	this->anchorpoint = anchorpoint;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	// 頂点データ
	VertexPosUv vertices[4];

	// 指定番号の画像が読み込み済みなら
	//if (this->spriteCommon->texBuff[this->texNumber]) {
	if (spriteCommon->GetTexBuff(this->texNumber)) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(this->texNumber)->GetDesc();

		// スプライトの大きさを画像の解像度に合わせる
		this->size = { (float)resDesc.Width, (float)resDesc.Height };
		this->texSize = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// 頂点バッファ生成
	result = spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&this->vertBuff));

	// 頂点バッファデータ転送
	TransferVertexBuffer();

	// 頂点バッファビューの作成
	this->vbView.BufferLocation = this->vertBuff->GetGPUVirtualAddress();
	this->vbView.SizeInBytes = sizeof(vertices);
	this->vbView.StrideInBytes = sizeof(vertices[0]);

	// 定数バッファの生成
	result = spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&this->constBuff));

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定（RGBA）
	constMap->mat = spriteCommon->GetMatProjection(); // 平行投影行列の合成
	this->constBuff->Unmap(0, nullptr);

}

void Sprite::TransferVertexBuffer()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	HRESULT result = S_FALSE;

	// 頂点データ
	VertexPosUv vertices[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // 左下
		{{}, {0.0f, 0.0f}}, // 左上
		{{}, {1.0f, 1.0f}}, // 右下
		{{}, {1.0f, 0.0f}}, // 右上
	};

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - this->anchorpoint.x) * this->size.x;
	float right = (1.0f - this->anchorpoint.x) * this->size.x;
	float top = (0.0f - this->anchorpoint.y) * this->size.y;
	float bottom = (1.0f - this->anchorpoint.y) * this->size.y;

	if (this->isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (this->isFlipY)
	{// 左右入れ替え
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left, bottom,  0.0f }; // 左下
	vertices[LT].pos = { left, top,     0.0f }; // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right, top,    0.0f }; // 右上

	// 指定番号の画像が読み込み済みなら
	if (spriteCommon->GetTexBuff(this->texNumber)) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(this->texNumber)->GetDesc();

		float tex_left = this->texLeftTop.x / resDesc.Width;
		float tex_right = (this->texLeftTop.x + this->texSize.x) / resDesc.Width;
		float tex_top = this->texLeftTop.y / resDesc.Height;
		float tex_bottom = (this->texLeftTop.y + this->texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,   tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,   tex_top }; // 左上
		vertices[RB].uv = { tex_right,  tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,  tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = this->vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	this->vertBuff->Unmap(0, nullptr);
}

void Sprite::Update()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	// Z軸回転
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(this->rotation));
	// 平行移動
	this->matWorld *= XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	// 定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = this->matWorld * spriteCommon->GetMatProjection();
	constMap->color = this->color;
	this->constBuff->Unmap(0, nullptr);
}

void Sprite::Draw()
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
