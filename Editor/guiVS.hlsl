// simple vertex shader
struct VSInput
{
	float2 position : POSITION;
	float4 color : COLOR;
	float2 uv : UV;
};
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};
cbuffer ConstantBuffer : register(b0)
{
	float4 screen;
	float4x4 rect;
	float4 color;
};
PSInput main(VSInput vs)
{
	PSInput res;
	res.position.w = 1.0f;
	res.position.z = 1.0f;
	res.position.xy = mul(rect, float4(vs.position, 0.0f, 0.0f)).xy;
	res.position.x = vs.position.x*screen.x*2 - 1;
	res.position.y = -vs.position.y*screen.y*2 + 1;
	res.uv = vs.uv;
	res.color = vs.color;
	return res;
}