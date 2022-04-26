#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>

struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node* parent = nullptr;
};
class Fbx_Model
{
public:
	//フレンドクラス
	friend class FbxLoader;

public://サブクラス
	//頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		//UINT boneIndex[MAX_BONE_INDICES];//影響を受けるボーン番号
		//float boneWeight[MAX_BONE_INDICES];//ボーン重み
	};

	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	//頂点インデックス配列
	std::vector<unsigned short> indices;
private://メンバ変数
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1, 1, 1 };
	//ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };
	//テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
};

