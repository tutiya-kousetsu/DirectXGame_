#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);   // 0番スロットに設定されたテクスチャ

SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

struct PSSIn
{
	float4 pos : SV_POSITION;	//スクリーン空間でのピクセルの座標
	float3 normal : NORMAL;		//法線
	float3 tangent : TANGENT;	//線ベクトル
	float3 biNormal : BINORMAL;	//従法線ベクトル
	float2 uv : TEXCOORD0;		//UV座標
	float3 worldPos : TEXCOORD1;//ワールド空間でのピクセルの座標

	float3 depthInview : TEXCOORD2;//カメラ空間でのZ値

};

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

float4 Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma / sigma));
}

PSOutput main(VSOutput input) 
{
	PSOutput output;

	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);
	float4 color = colortex0;
	output.target1 = float4(1.0f - color.rgb, 1);

	/*float u = 1.0f / 1280.0f;
	float v = 1.0f / 720.0f;
	int count = 10;
	
	for (int i = 0; i < count * 2 + 1; i++)
	{
		for (int j = 0; j < count * 2 + 1; j++)
		{
			color += tex0.Sample(smp,
				input.uv - float2(u * count, v * count) + float2(u * i, v * j));
		}
	}
	
	output.target0 = float4(color.rgb / ((count * 2 + 1) * (count * 2 + 1)), 1);*/

	/*モノクロ化*/
	/*float Y = 0.299f * colortex0.r + 0.587f * colortex0.b + 0.114f * colortex0.b;

	colortex0.r = Y;
	colortex0.g = Y;
	colortex0.b = Y;
	output.target0 = colortex0;*/

	/*平均ブラー*/
	/*float offsetU = 1.0f / 1280.0f;
	float offsetV = 1.0f / 720.0f;

	colortex0 += tex0.Sample(smp, input.uv + float2(offsetU, 0.0f));
	colortex0 += tex0.Sample(smp, input.uv + float2(-offsetU, 0.0f));
	colortex0 += tex0.Sample(smp, input.uv + float2(0.0f, offsetV));
	colortex0 += tex0.Sample(smp, input.uv + float2(0.0f, -offsetV));
	colortex0 += tex0.Sample(smp, input.uv + float2(offsetU, offsetV));
	colortex0 += tex0.Sample(smp, input.uv + float2(offsetU, -offsetV));
	colortex0 += tex0.Sample(smp, input.uv + float2(-offsetU, offsetV));
	colortex0 += tex0.Sample(smp, input.uv + float2(-offsetU, -offsetV));

	colortex0 /= 9.0f;*/

	float totalWeight = 0.0f, _Sigma = 0.005f, _StepWidth = 0.001f;
	float4 col = float4(0, 0, 0, 0);
	for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth) {
		for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth) {
			float2 pickUV = input.uv + float2(px, py);
			float4 weight = Gaussian(input.uv, pickUV, _Sigma);
			col += tex0.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}

	col.rgb = col.rgb / totalWeight;
	
	output.target0 = col;

	//output.target0 = colortex0;
	//アルファに1を入れて出力
	return output;
}
