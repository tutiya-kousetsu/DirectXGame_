#pragma once

#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include <DirectXMath.h>

/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GamePlayScene
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
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	void Finalize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
	Object3d* objPost2 = nullptr;
	DebugCamera* camera = nullptr;

	float speed = 0.75f;
	float t = 0.01f;
	float speed2 = 0.15f;
	float g = 0.5f;
	int Flag = 0;
	float r;
	float m = 1.f;
	float repulCoe = 0.8f;    // ���˕Ԃ�W��
	float Angle = 15.0f;             //�p�x
	float Length = 20.0f;                 //���a�̒���
	float centerX = 0;			//���S���Wx
	float centerY = 0;			//���S���Wy
	XMFLOAT3 playerPos;
	XMFLOAT3 playerScale;
	XMFLOAT3 playerPos2;
	XMFLOAT3 playerScale2;
	XMFLOAT3 v;
	XMFLOAT3 a;

};

