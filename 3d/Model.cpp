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
	//新たなModel型のインスタンスのメモリを確保
	Model* model = new Model();

	//デスクリプタヒープの生成
	model->InitializeDescriptorHeap();

	//OBJファイルからデータを読み込み
	model->LoadFromOBJInternal(modelname);

	//バッファの生成
	model->CreateBuffers();

	return model;
}

bool Model::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
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

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&this->texbuff));

	if (FAILED(result)) {
		return result;
	}

	// テクスチャバッファにデータ転送
	result = this->texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		return result;
	}

	// シェーダリソースビュー作成
	this->cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(this->descHeap->GetCPUDescriptorHandleForHeapStart(), 0, this->descriptorHandleIncrementSize);
	this->gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(this->descHeap->GetGPUDescriptorHandleForHeapStart(), 0, this->descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = this->texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(this->texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		this->cpuDescHandleSRV
	);

	return true;
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial)
{
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&this->ibView);
	// マテリアル用定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, this->constBuffB1->GetGPUVirtualAddress());

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { this->descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (this->material.textureFilename.size() > 0) {
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(2, this->gpuDescHandleSRV);
	}
	// 描画コマンド
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

	std::vector<XMFLOAT3>positions;//頂点座標
	std::vector<XMFLOAT3>normals;//法線ベクトル
	std::vector<XMFLOAT2>texcoords;//テクスチャUV
	//1行ずつ読み込む

	std::string line;
	while (getline(file, line)) {
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//半角SPACE区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		//先頭文字がvなら頂点座標
		if (key == "v") {
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
			//頂点データに追加
			//VertexPosNormalUv vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);

		}
		//先頭文字列がvtならテクスチャ
		if (key == "vt") {
			//UV成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//v方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		//先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			//XYZ成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			std::string filename;
			line_stream >> filename;
			LoadMaterial(directoryPath, filename);
		}
		//先頭文字がfならポリゴン
		if (key == "f") {
			//半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//頂点インデックス1行分の文字列をストリームに変換
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;
				//頂点インデックスに追加]
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				this->vertices.emplace_back(vertex);

				this->indices.emplace_back((unsigned short)this->indices.size());
			}
		}
	}
	//ファイル閉じる
	file.close();
	//return nullptr;
}
void Model::InitializeDescriptorHeap()
{
	assert(device);

	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&this->descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// デスクリプタサイズを取得
	this->descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}
void Model::CreateBuffers()
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * this->vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * this->indices.size());
	// 頂点バッファ生成
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

	// インデックスバッファ生成
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

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = this->vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(this->vertices.begin(), this->vertices.end(), vertMap);
		this->vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = this->indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	if (SUCCEEDED(result)) {
		std::copy(this->indices.begin(), this->indices.end(), indexMap);
		this->indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	this->vbView.BufferLocation = this->vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeof(vertices);
	this->vbView.SizeInBytes = sizeVB;
	this->vbView.StrideInBytes = sizeof(this->vertices[0]);

	// インデックスバッファビューの作成
	this->ibView.BufferLocation = this->indexBuff->GetGPUVirtualAddress();
	this->ibView.Format = DXGI_FORMAT_R16_UINT;
	this->ibView.SizeInBytes = sizeIB;

	// マテリアル用定数バッファの生成B1
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->constBuffB1));
	assert(SUCCEEDED(result));

	//マテリアル用定数バッファへデータ転送
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
