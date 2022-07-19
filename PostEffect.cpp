#include "PostEffect.h"
#include "PipelineSet.h"
#include "Sprite.h"

#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect()
	:Sprite(
		100,//�e�N�X�`���ԍ�
		{ 0, 0 },//���W
		{ 500.0f, 500.0f },//�T�C�Y
		{ 1, 1, 1, 1 },//�F
		{ 0.0f, 0.0f },//�A���J�[�|�C���g
		false,//���E���]�t���O
		false)//�㉺���]�t���O
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;	// �s��̍���	
		this->constBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));
	// �`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}
