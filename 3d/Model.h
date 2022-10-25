#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

/// <summary>
/// 3D���f��
/// </summary>
class Model
{
public:
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv;  // uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};

	//�}�e���A��
	struct Material
	{
		std::string name;//�}�e���A����
		DirectX::XMFLOAT3 ambient;//�A���r�G���g�e���x
		DirectX::XMFLOAT3 diffuse;//�f�B�q���[�Y�e���x
		DirectX::XMFLOAT3 specular;//�X�y�L�����e���x
		float alpha;//
		std::string textureFilename;//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public:
	/// <summary>
	/// OBJ�t�@�C������D���f����ǂݍ���
	/// </summary>
	/// <param name="modelname">���f����</param>
	/// <returns>���f��</returns>
	static Model* LoadFromObj(const std::string& modelname);

	/// <summary>
	/// �}�e���A���̓ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="rootParamIndexMaterial">�}�e���A���̃��[�g�p�����[�^�ԍ��w��</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

	static void SetDevice(ID3D12Device* device) { Model::device = device; }

private: // �����o�֐�
	//�f�o�C�X(�؂�Ă���)
	static ID3D12Device* device;

	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv>vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short>indices;

	//�}�e���A��
	Material material;

	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;

	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	//�}�e���A���p�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

private:
	void LoadFromOBJInternal(const std::string& modelname);

	// �f�X�N���v�^�q�[�v�̏�����
	void InitializeDescriptorHeap();

	//�e��o�b�t�@�[�̐���
	void CreateBuffers();
};

