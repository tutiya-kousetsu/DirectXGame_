#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp : register(s0);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSSIn
{
	float4 pos : SV_POSITION;	//�X�N���[����Ԃł̃s�N�Z���̍��W
	float3 normal : NORMAL;		//�@��
	float3 tangent : TANGENT;	//���x�N�g��
	float3 biNormal : BINORMAL;	//�]�@���x�N�g��
	float2 uv : TEXCOORD0;		//UV���W
	float3 worldPos : TEXCOORD1;//���[���h��Ԃł̃s�N�Z���̍��W

	float3 depthInview : TEXCOORD2;//�J������Ԃł�Z�l

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

	/*���m�N����*/
	/*float Y = 0.299f * colortex0.r + 0.587f * colortex0.b + 0.114f * colortex0.b;

	colortex0.r = Y;
	colortex0.g = Y;
	colortex0.b = Y;
	output.target0 = colortex0;*/

	/*���σu���[*/
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
	//�A���t�@��1�����ďo��
	return output;
}
