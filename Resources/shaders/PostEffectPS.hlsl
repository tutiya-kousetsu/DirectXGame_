#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);   // 0番スロットに設定されたテクスチャ

SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
   // float4 texcolor = tex.Sample(smp, input.uv);
   // return float4(texcolor.rgb, 1);

	//float4 texcolor = tex.Sample(smp, input.uv);
	//float4 texcolor = tex.Sample(smp, input.uv + 0.1f);
	//明度の変更
	//return float4(texcolor.rgb * 2.0f, 1);
	//色反転
	//return float4(1.0f - texcolor.rgb, 1);

	//UV指定したピクセルの色をサンプリング
	//float4 texcolor = tex.Sample(smp, input.uv)

	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	float u = 1.0f / 1280.0f;
	float v = 1.0f / 720.0f;
	//float4 color = { 0,0,0,0 };
	int count = 20;

	float4 color = colortex0;
	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = colortex1;
		for (int i = 0; i < count * 2 + 1; i++)
		{
			for (int j = 0; j < count * 2 + 1; j++)
			{
				color += tex0.Sample(smp,
					input.uv - float2(u * count, v * count) + float2(u * i, v * j));
			}
		}
		return float4(color.rgb / ((count * 2 + 1) * (count * 2 + 1)), 1);
	}

	//アルファに1を入れて出力
	return float4(1.0f - color.rgb, 1);



	//平均ぼかし
	/*float u = 1.0f / 1280.0f;
	float v = 1.0f / 720.0f;
	float4 color = { 0,0,0,0 };
	int count = 20;
	for (int i = 0; i < count * 2 + 1; i++)
	{
		for (int j = 0; j < count * 2 + 1; j++)
		{
			color += tex.Sample(smp,
				input.uv - float2(u * count, v * count) + float2(u * i, v * j));
		}
	}
	return float4(color.rgb / ((count * 2 + 1) * (count * 2 + 1)), 1);*/
}
