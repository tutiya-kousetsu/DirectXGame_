#pragma once
#include<wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInpu
#include<dinput.h>
#include"WinApp.h"

//����
class Input final {
public:
	//namespace�ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�����o�֐�
	static Input* GetInstance();
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name = "KeyNumber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name = "KeyNumber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool TriggerKey(BYTE keyNumber);

private:
	//�R���X�g���N�^
	Input() = default;
	//�f�X�g���N�^
	~Input() = default;
public:
	//�R�s�[�R���X�g���N�^�𖳌��ɂ���
	Input(const Input& obj) = delete;

	Input& operator=(const Input& obj) = delete;
private://�����o�ϐ�
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> dinput;
	//�S�L�[�̏��
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	//WindowsAPI
	WinApp* winApp = nullptr;
};