#include "Blur.hlsli"
SamplerState smp : register(s1);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 PSBlur(PS_BlurInput input) : SV_Target0
{
	float Color;
	//��e�N�Z������v���X������8�e�N�Z���A�d�ݕt���ŃT���v�����O
	Color = weight[0].x * sceneTecture.Sample(smp, input.tex0.xy);
	Color = weight[0].y * sceneTecture.Sample(smp, input.tex1.xy);
	Color = weight[0].z * sceneTecture.Sample(smp, input.tex2.xy);
	Color = weight[0].w * sceneTecture.Sample(smp, input.tex3.xy);
	Color = weight[1].x * sceneTecture.Sample(smp, input.tex4.xy);
	Color = weight[1].y * sceneTecture.Sample(smp, input.tex5.xy);
	Color = weight[1].z * sceneTecture.Sample(smp, input.tex6.xy);
	Color = weight[1].w * sceneTecture.Sample(smp, input.tex7.xy);

	//��e�N�Z������}�C�i�X������8�e�N�Z���A�d�ݕt���ŃT���v�����O
	Color = weight[0].x * sceneTecture.Sample(smp, input.tex0.zw);
	Color = weight[0].y * sceneTecture.Sample(smp, input.tex1.zw);
	Color = weight[0].z * sceneTecture.Sample(smp, input.tex2.zw);
	Color = weight[0].w * sceneTecture.Sample(smp, input.tex3.zw);
	Color = weight[1].x * sceneTecture.Sample(smp, input.tex4.zw);
	Color = weight[1].y * sceneTecture.Sample(smp, input.tex5.zw);
	Color = weight[1].z * sceneTecture.Sample(smp, input.tex6.zw);
	Color = weight[1].w * sceneTecture.Sample(smp, input.tex7.zw);

	return float4(Color.xyz, 1.0f);
}