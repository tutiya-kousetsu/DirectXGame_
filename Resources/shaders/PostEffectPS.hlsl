#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);   // 0番スロットに設定されたテクスチャ

SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input) 
{
	PSOutput output;

	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	float u = 1.0f / 1280.0f;
	float v = 1.0f / 720.0f;
	int count = 20;
	
	float4 color = colortex0;
	output.target1 = float4(1.0f - color.rgb, 1);
	for (int i = 0; i < count * 2 + 1; i++)
	{
		for (int j = 0; j < count * 2 + 1; j++)
		{
			color += tex0.Sample(smp,
				input.uv - float2(u * count, v * count) + float2(u * i, v * j));
		}
	}
	output.target0 = float4(color.rgb / ((count * 2 + 1) * (count * 2 + 1)), 1);

	//アルファに1を入れて出力
	return output;
}
