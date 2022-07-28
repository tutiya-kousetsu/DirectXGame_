//ブラー用の定数バッファ
cbuffer CBBlur : register(b1)
{
	float4 weight[2];//重み
}