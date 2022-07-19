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

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	spriteCommon->GetCommandList();

	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	// Z����]
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(this->rotation));
	// ���s�ړ�
	this->matWorld *= XMMatrixTranslation(this->position.x, this->position.y, 0.0f);

	// �萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = this->matWorld * spriteCommon->GetMatProjection();
	constMap->color = this->color;
	this->constBuff->Unmap(0, nullptr);

	spriteCommon->PreDraw();


	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// ���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	// ���[�g�p�����[�^1�ԂɃV�F�[�_���\�[�X�r���[���Z�b�g
	spriteCommon->SetGraphicsRootDescriptorTable(1, this->texNumber);
	/*cmdList->SetPipelineState(pipelineSet->pipelinestate.Get());

	cmdList->SetGraphicsRootSignature(pipelineSet->rootsignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);*/

	// �|���S���̕`��i4���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);

}
