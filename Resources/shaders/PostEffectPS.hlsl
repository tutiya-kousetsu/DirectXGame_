#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp : register(s0);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

float4 Gaussian(float2 drawUv, float2 pickUv, float sigma)
{
	float d = distance(drawUv, pickUv);
	return exp(-(d * d) / (2 * sigma * sigma));
}

PSOutput main(VSOutput input) 
{
	PSOutput output;

	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);
	float4 color = colortex0;
	//���]
	output.target1 = float4(1.0f - color.rgb, 1);

	float totalWeight = 0.0f, sigma = 0.005f, stepWidth = 0.001f;
	float4 col = float4(0, 0, 0, 1);
	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth) {
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth) {
			float2 pickUv = input.uv + float2(px, py);
			float4 weight = Gaussian(input.uv, pickUv, sigma);
			col += tex0.Sample(smp, pickUv) * weight;
			totalWeight += weight;
		}
	}

	col.rgb = col.rgb / totalWeight;

	output.target0 = col;

	return output;

}
