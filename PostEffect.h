#pragma once
#include "Sprite.h"

class PostEffect :
	public Sprite
{
protected://�G�C���A�X
//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:
	//�f�o�C�X(�؂�Ă���)
	ID3D12Device* device = nullptr;
	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//RTV�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};

