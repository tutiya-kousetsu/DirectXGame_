cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
};

//バーテックスバッファーの入力
struct VSInput
{
	float4 pos : POSITION;//位置
	float3 normal :NORMAL;//頂点法線
	float2 uv : TEXCOORD;//テクスチャー座標
	//uint4 boneIndices : BONEINDICES;//ボーン番号
	//float4 boneWeights : BONEWEIGHTS;//ボーンのスキンウェイト
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};