#include "Blur.hlsli"

PS_BlurInput VSYBlur(VSInput input)
{
	PS_BlurInput output;
	//���W�ϊ�
	output.pos = mul(mvp, input.pos);

	//�e�N�X�`���T�C�Y���擾
	float2 texSize;
	float level;
	sceneTexture.GetDimension(0, texSize.x, tetexSize.y, level);

	//��e�N�Z����UV���W���L�^
	float2 tex = input.uv;

	//��e�N�Z��U���W���{1�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
	output.tex0.xy = float2(1.0f / texSize.y, 0.0f);

	//��e�N�Z��U���W���{3�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
	output.tex1.xy = float2(3.0f / texSize.y, 0.0f);

	//��e�N�Z��U���W���{5�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
	output.tex2.xy = float2(5.0f / texSize.y, 0.0f);

	//��e�N�Z��U���W���{7�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
	output.tex3.xy = float2(7.0f / texSize.y, 0.0f);

	//��e�N�Z��U���W���{9�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
	output.tex4.xy = float2(9.0f / texSize.y, 0.0f);

	//��e�N�Z��U���W���{11�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
	output.tex5.xy = float2(11.0f / texSize.y, 0.0f);

	//��e�N�Z��U���W���{13�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
	output.tex6.xy = float2(13.0f / texSize.y, 0.0f);

	//��e�N�Z��U���W���{15�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
	output.tex7.xy = float2(15.0f / texSize.y, 0.0f);

	//�I�t�Z�b�g��-1���|���ă}�C�i�X�����̃I�t�Z�b�g���v�Z����
	output.tex0.zw = output.tex0.xy * -1.0f;
	output.tex1.zw = output.tex1.xy * -1.0f;
	output.tex2.zw = output.tex2.xy * -1.0f;
	output.tex3.zw = output.tex3.xy * -1.0f;
	output.tex4.zw = output.tex4.xy * -1.0f;
	output.tex5.zw = output.tex5.xy * -1.0f;
	output.tex6.zw = output.tex6.xy * -1.0f;
	output.tex7.zw = output.tex7.xy * -1.0f;

	//�I�t�Z�b�g�Ɋ�e�N�Z����UV���W�𑫂��Z���āA���ۂɃT���v�����O����UV���W�ɕϊ�����
	output.tex0 += float4(tex, tex);
	output.tex1 += float4(tex, tex);
	output.tex2 += float4(tex, tex);
	output.tex3 += float4(tex, tex);
	output.tex4 += float4(tex, tex);
	output.tex5 += float4(tex, tex);
	output.tex6 += float4(tex, tex);
	output.tex7 += float4(tex, tex);
	return output
}

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}