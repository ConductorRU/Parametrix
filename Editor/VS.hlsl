// simple vertex shader
struct VSInput
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 uv : UV;
};
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 wposition : TEXCOORD1;
	float2 uv : TEXCOORD2;
};
cbuffer ConstantBuffer : register(b0)
{
	float4x4 wvpMat;
	float4x4 worldMat;
};
PSInput main(VSInput vs)
{
	PSInput res;
	res.position = mul(wvpMat, float4(vs.position, 1.0f));//mul(input.pos, wvpMat);
	res.color = vs.color;
	res.normal = normalize(mul((float3x3)worldMat, vs.normal));
  res.wposition = (float3)mul(worldMat, float4(vs.position, 1.0f));
	res.uv = vs.uv;
	return res;
}