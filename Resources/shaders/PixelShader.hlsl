#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);   // 0番スロットに設定されたテクスチャ

SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	/*if (fmod(input.uv.y, 0.1f) < 0.05f) {
		 return tex0.Sample(smp, input.uv);
	}*/
	return tex0.Sample(smp, input.uv);
}