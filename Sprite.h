#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

#include"SpriteCommon.h"

/// <summary>
/// �X�v���C�g1�����̃f�[�^
/// </summary>
class Sprite
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	// ���_�f�[�^
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT2 uv;  // uv���W
	};
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		DirectX::XMFLOAT4 color; // �F (RGBA)
		DirectX::XMMATRIX mat;   // �R�c�ϊ��s��
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">X���]���邩</param>
	/// <param name="isFlipY">Y���]���邩</param>
	static Sprite* Create(UINT texNumber, DirectX::XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���_�o�b�t�@�̓]��
	/// </summary>
	void TransferVertexBuffer();

	/// <summary>
	/// ���t���[���X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; }

	void SetRotation(float rotation) { this->rotation = rotation; }

	void SetSize(const DirectX::XMFLOAT2& size) { this->size = size; }

	void SetTexLeftTop(const DirectX::XMFLOAT2& texLeftTop) { this->texLeftTop = texLeftTop; }

	void SetTexSize(const DirectX::XMFLOAT2& texSize) { this->texSize = texSize; }

protected:
	//���_�o�b�t�@;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	DirectX::XMFLOAT2 position = { 0,0 };
	// ���[���h�s��
	DirectX::XMMATRIX matWorld;
	// �F(RGBA)
	DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
	// �e�N�X�`���ԍ�
	UINT texNumber = 0;
	// �傫��
	DirectX::XMFLOAT2 size = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	DirectX::XMFLOAT2 anchorpoint = { 0, 0 };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`��������W
	DirectX::XMFLOAT2 texLeftTop = { 0, 0 };
	// �e�N�X�`���؂�o���T�C�Y
	DirectX::XMFLOAT2 texSize = { 100.0f, 100.0f };
	//��\��
	bool isInvisible = false;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �p�C�v���C���Z�b�g
	PipelineSet* pipelineSet = nullptr;
};

