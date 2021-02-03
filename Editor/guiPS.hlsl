// simple pixel shader
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};
float4 main(PSInput input): SV_TARGET
{
	float4 txCol = input.color;
	return input.color;
}