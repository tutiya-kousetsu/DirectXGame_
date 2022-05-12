#pragma once
#include "DirectXCommon.h"

/// <summary>
/// �V�[���C���^�[�t�F�[�X
/// </summary>
class BaseScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon) = 0;

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon) = 0;
};

