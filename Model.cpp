#include "Model.h"
#include<string>
#include <fstream>
#include<Windows.h>
#include<cassert>
#include<vector>
#include<DirectXMath.h>
#include <sstream>
#include <DirectXTex.h>

using namespace DirectX;

ID3D12Device* Model::device = nullptr;

Model* Model::LoadFromObj(const std::string& modelname)
{
	//�V����Model�^�̃C���X�^���X�̃��������m��
	Model* model = new Model();

	//�f�X�N���v�^�q�[�v�̐���
	model->InitializeDescriptorHeap();

	//OBJ�t�@�C������f�[�^��ǂݍ���
	model->LoadFromOBJInternal(modelname);

	//�o�b�t�@�̐���
	model->CreateBuffers();

	return model;
}

bool Model::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	std::string filepath = directoryPath + filename;

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
	directoryPath + filename;
	/*result = LoadFromWICFile(
		L"Resources/tex1.png", WIC_FLAGS_NONE,
		&metadata, scratchImg);*/
	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	if (FAILED(result)) {
		return result;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&this->texbuff));

	if (FAILED(result)) {
		return result;
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = this->texbuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);
	if (FAILED(result)) {
		return result;
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	this->cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(this->descHeap->GetCPUDescriptorHandleForHeapStart(), 0, this->descriptorHandleIncrementSize);
	this->gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(this->descHeap->GetGPUDescriptorHandleForHeapStart(), 0, this->descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = this->texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(this->texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		this->cpuDescHandleSRV
	);

	return true;
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial)
{
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&this->ibView);
	// �}�e���A���p�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, this->constBuffB1->GetGPUVirtualAddress());

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { this->descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (this->material.textureFilename.size() > 0) {
		// �V�F�[�_���\�[�X�r���[���Z�b�g
		cmdList->SetGraphicsRootDescriptorTable(2, this->gpuDescHandleSRV);
	}
	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)this->indices.size(), 1, 0, 0, 0);
}

void Model::LoadFromOBJInternal(const std::string& modelname)
{
	//const std::string modelname = "post";//skydome,post
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = "Resources/" + modelname + "/";

	//HRESULT result = S_FALSE;

	std::ifstream file;

	file.open(directoryPath + filename);
	if (file.fail()) {
		assert(0);
	}

	std::vector<XMFLOAT3>positions;//���_���W
	std::vector<XMFLOAT3>normals;//�@���x�N�g��
	std::vector<XMFLOAT2>texcoords;//�e�N�X�`��UV
	//1�s���ǂݍ���

	std::string line;
	while (getline(file, line)) {
		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);
		//���pSPACE��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		//�擪������v�Ȃ璸�_���W
		if (key == "v") {
			//X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
			//���_�f�[�^�ɒǉ�
			//VertexPosNormalUv vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);

		}
		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt") {
			//UV�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//v�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn") {
			//XYZ�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}
		//�擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			//�}�e���A���̃t�@�C�����ǂݍ���
			std::string filename;
			line_stream >> filename;
			LoadMaterial(directoryPath, filename);
		}
		//�擪������f�Ȃ�|���S��
		if (key == "f") {
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//���_�C���f�b�N�X1�s���̕�������X�g���[���ɕϊ�
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;
				//���_�C���f�b�N�X�ɒǉ�]
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				this->vertices.emplace_back(vertex);

				this->indices.emplace_back((unsigned short)this->indices.size());
			}
		}
	}
	//�t�@�C������
	file.close();
	//return nullptr;
}
void Model::InitializeDescriptorHeap()
{
	assert(device);

	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&this->descHeap));//����
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// �f�X�N���v�^�T�C�Y���擾
	this->descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}
void Model::CreateBuffers()
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * this->vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * this->indices.size());
	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = this->vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(this->vertices.begin(), this->vertices.end(), vertMap);
		this->vertBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = this->indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	if (SUCCEEDED(result)) {
		std::copy(this->indices.begin(), this->indices.end(), indexMap);
		this->indexBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	this->vbView.BufferLocation = this->vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeof(vertices);
	this->vbView.SizeInBytes = sizeVB;
	this->vbView.StrideInBytes = sizeof(this->vertices[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	this->ibView.BufferLocation = this->indexBuff->GetGPUVirtualAddress();
	this->ibView.Format = DXGI_FORMAT_R16_UINT;
	this->ibView.SizeInBytes = sizeIB;

	// �}�e���A���p�萔�o�b�t�@�̐���B1
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->constBuffB1));
	assert(SUCCEEDED(result));

	//�}�e���A���p�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap1 = nullptr;
	result = this->constBuffB1->Map(0, nullptr, (void**)&constMap1);
	assert(SUCCEEDED(result));
	constMap1->ambient = this->material.ambient;
	constMap1->diffuse = this->material.diffuse;
	constMap1->specular = this->material.specular;
	constMap1->alpha = this->material.alpha;
	this->constBuffB1->Unmap(0, nullptr);
}
void  Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	std::ifstream file;
	file.open(directoryPath + filename);
	if (file.fail()) {
		assert(0);
	}

	//Model* material = nullptr;

	std::string line;
	while (getline(file, line)) {
		std::istringstream line_stream(line);
		std::string key;
		getline(line_stream, key, ' ');
		if (key[0] == '\t') {
			key.erase(key.begin());
		}
		if (key == "usemtl") {
			line_stream >> this->material.name;
		}
		if (key == "Ka") {
			line_stream >> this->material.ambient.x;
			line_stream >> this->material.ambient.y;
			line_stream >> this->material.ambient.z;
		}
		if (key == "Kd") {
			line_stream >> this->material.diffuse.x;
			line_stream >> this->material.diffuse.y;
			line_stream >> this->material.diffuse.z;
		}
		if (key == "Ks") {
			line_stream >> this->material.specular.x;
			line_stream >> this->material.specular.y;
			line_stream >> this->material.specular.z;
		}
		if (key == "map_Kd") {
			line_stream >> this->material.textureFilename;
			LoadTexture(directoryPath, this->material.textureFilename);
		}
	}
	file.close();
}
