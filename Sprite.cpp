#include "Sprite.h"
#include <d3dx12.h>

using namespace DirectX;

Sprite* Sprite::Create(UINT texNumber, DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	//�������m��
	Sprite* instance = new Sprite;
	//�C���X�^���X������
	instance->Initialize(texNumber, anchorpoint, isFlipX, isFlipY);
	
	return instance;
}

void Sprite::Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;

	//�����o�ϐ��ɏ�������
	this->texNumber = texNumber;
	this->anchorpoint = anchorpoint;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	// ���_�f�[�^
	VertexPosUv vertices[4];

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	//if (this->spriteCommon->texBuff[this->texNumber]) {
	if (spriteCommon->GetTexBuff(this->texNumber)) {
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(this->texNumber)->GetDesc();

		// �X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		this->size = { (float)resDesc.Width, (float)resDesc.Height };
		this->texSize = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// ���_�o�b�t�@����
	result = spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&this->vertBuff));

	// ���_�o�b�t�@�f�[�^�]��
	TransferVertexBuffer();

	// ���_�o�b�t�@�r���[�̍쐬
	this->vbView.BufferLocation = this->vertBuff->GetGPUVirtualAddress();
	this->vbView.SizeInBytes = sizeof(vertices);
	this->vbView.StrideInBytes = sizeof(vertices[0]);

	// �萔�o�b�t�@�̐���
	result = spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&this->constBuff));

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // �F�w��iRGBA�j
	constMap->mat = spriteCommon->GetMatProjection(); // ���s���e�s��̍���
	this->constBuff->Unmap(0, nullptr);

}

void Sprite::TransferVertexBuffer()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	HRESULT result = S_FALSE;

	// ���_�f�[�^
	VertexPosUv vertices[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // ����
		{{}, {0.0f, 0.0f}}, // ����
		{{}, {1.0f, 1.0f}}, // �E��
		{{}, {1.0f, 0.0f}}, // �E��
	};

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - this->anchorpoint.x) * this->size.x;
	float right = (1.0f - this->anchorpoint.x) * this->size.x;
	float top = (0.0f - this->anchorpoint.y) * this->size.y;
	float bottom = (1.0f - this->anchorpoint.y) * this->size.y;

	if (this->isFlipX)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (this->isFlipY)
	{// ���E����ւ�
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left, bottom,  0.0f }; // ����
	vertices[LT].pos = { left, top,     0.0f }; // ����
	vertices[RB].pos = { right, bottom, 0.0f }; // �E��
	vertices[RT].pos = { right, top,    0.0f }; // �E��

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (spriteCommon->GetTexBuff(this->texNumber)) {
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(this->texNumber)->GetDesc();

		float tex_left = this->texLeftTop.x / resDesc.Width;
		float tex_right = (this->texLeftTop.x + this->texSize.x) / resDesc.Width;
		float tex_top = this->texLeftTop.y / resDesc.Height;
		float tex_bottom = (this->texLeftTop.y + this->texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,   tex_bottom }; // ����
		vertices[LT].uv = { tex_left,   tex_top }; // ����
		vertices[RB].uv = { tex_right,  tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,  tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = this->vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	this->vertBuff->Unmap(0, nullptr);
}

void Sprite::Update()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	// Z����]
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(this->rotation));
	// ���s�ړ�
	this->matWorld *= XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	// �萔�o�b�t�@�̓]��
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

	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// ���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	// ���[�g�p�����[�^1�ԂɃV�F�[�_���\�[�X�r���[���Z�b�g
	spriteCommon->SetGraphicsRootDescriptorTable(1, this->texNumber);
	
	// �|���S���̕`��i4���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);
}
