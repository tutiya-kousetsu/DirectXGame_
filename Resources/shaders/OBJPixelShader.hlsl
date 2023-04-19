#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//ライトの向き
	float3 light = normalize(float3(0,-1,0));
	//diffuseを[0,1]の範囲に固定する
	float diffuse = saturate(dot(-light, input.normal));
	//アンビエント項を0.7にする
	float brightness = diffuse + 0.7f;
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	return float4(texcolor.rgb * brightness , texcolor.a) * color.w;
}