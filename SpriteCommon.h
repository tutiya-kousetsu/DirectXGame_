#pragma once

#include"PipelineSet.h"

#include <DirectXMath.h>

/// <summary>
/// �X�v���C�g����
/// </summary>

class SpriteCommon
{
public:
	static SpriteCommon* GetInstance();

	// �e�N�X�`���̍ő喇��
	static const int spriteSRVCount = 512;

/// <summary>
/// ������
/// </summary>
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture(UINT texnumber, const wchar_t* filename);

	/// <summary>
	/// ���[�g�f�X�N���v�^�e�[�u���̐ݒ�
	/// </summary>
	/// <param name="rootParameterIndex">���[�g�p�����[�^�ԍ�</param>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber);

	/// <summary>
	/// �e�N�X�`���擾
	/// </summary>
	/// <param name="texnumber">�ԍ�</param>
	/// <returns></returns>
	ID3D12Resource* GetTexBuff(int texNumber);

	ID3D12Device* GetDevice() { return device; }

	const DirectX::XMMATRIX& GetMatProjection() { return matProjection; }

	ID3D12GraphicsCommandList* GetCommandList() {return cmdList;}

protected:
	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	// �ˉe�s��
	DirectX::XMMATRIX matProjection{};
	// �e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j�̔z��
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	//�f�o�C�X(�؂�Ă���)
	ID3D12Device* device = nullptr;

	ID3D12GraphicsCommandList* cmdList = nullptr;

protected:
	// �p�C�v���C������
	void CreateGraphicsPipeline();

};

