#include "FbxLoader.h"
#include <cassert>

using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実態
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
	//再初期化チェック
	assert(fbxManager == nullptr);
	//引数からメンバ変数に代入
	this->device = device;
	//FBXマネージャの生成
	fbxManager = FbxManager::Create();
	//FBXマネージャの入出力設定
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);
	//FBXインポータの生成
	fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
	//各種FBXインスタンスの破棄
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

void FbxLoader::LoadModelFromFile(const string& modelName)
{
	//モデル名と同じ名前のフォルダから読み込む
	const string directoryPath = baseDirectory + modelName + "/";
	//拡張子.fbxを付加
	const string fileName = modelName + ".fbx";
	//連結してフルパスを得る
	const string fullpath = directoryPath + fileName;

	//ファイル名を指定してFBXファイルを読み込む
	if (!fbxImporter->Initialize(fullpath.c_str(),
		-1, fbxManager->GetIOSettings())) {
		assert(0);
	}

	//シーン生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

	//ファイルからロードしたFBXの情報をシーンにインポート
	fbxImporter->Import(fbxScene);

	//モデル生成
	Fbx_Model* fbx_model = new Fbx_Model();
	fbx_model->name = modelName;

	//FBXノードの数を取得
	int nodeCount = fbxScene->GetNodeCount();
	//あらかじめ必要分数のメモリを確保することで、アドレスがずれるのを予防
	fbx_model->nodes.reserve(nodeCount);

	//ルートノードから順に解析してモデルに流し込む
	ParseNodeRecursive(fbx_model, fbxScene->GetRootNode());
	//FBXシーン解放
	fbxScene->Destroy();
}

void FbxLoader::ParseNodeRecursive(Fbx_Model* fbx_model, FbxNode* fbxNode, Node* parent)
{
	//モデルにノードを追加
	fbx_model->nodes.emplace_back();
	Node& node = fbx_model->nodes.back();
	//ノード名を取得
	string name = fbxNode->GetName();

	//FBXノードのローカル移動情報
	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();
	node.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2],0.0f };
	node.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2],0.0f };
	node.translation = { (float)translation[0], (float)translation[1], (float)translation[2],1.0f };

	//回転角をDegree(度)からラジアンに変換
	node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

	//スケール、回転、平行移動行列の計算
	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);

	//ローカル変形行列の計算
	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;//ワールド行列にスケーリングを反映
	node.transform *= matRotation;//ワールド行列に回転を反映
	node.transform *= matTranslation;//ワールド行列に平行移動を反映

	//グローバル変形行列の計算
	node.globalTransform = node.transform;
	if (parent) {
		node.parent = parent;
		//親の変形を乗算
		node.globalTransform *= parent->globalTransform;
	}
	//Todo
	
	//子ノードに対して再帰呼び出し
	for (int i = 0; i < fbxNode->GetChildCount(); i++) {
		ParseNodeRecursive(fbx_model, fbxNode->GetChild(i), &node);
	}
}
