
Texture2D		texDiffuse;	// texture
SamplerState	samLinear;	// sample State ( filtering/mode )

struct VertexIn
{
	float3	pos		: POSITION;
	float4	color	: COLOR;

	float3	normal	: NORMAL;

	float2	tex		: TEXCOORD;
};

struct VertexOut
{
	float4	pos		: SV_POSITION;
	float4	color	: COLOR;

	float4	normal	: NORMAL;
	float2	tex		: TEXCOORD;
};

cbuffer ConstantBuffer
{
	float4x4    wvp;
	float4x4	world;

	float4		lightDir;
	float4		lightColor;
}

VertexOut    VS(VertexIn vIn)
{
	VertexOut vOut;

	vOut.pos = mul(float4(vIn.pos, 1.0f), wvp);
	vOut.color = vIn.color;

	vOut.normal = mul(float4(vIn.normal, 0.0f), world);

	/*vOut.tex = saturate(vIn.tex);*/
	vOut.tex = vIn.tex;

	return vOut;
}

float4 PS(VertexOut vOut) : SV_TARGET
{
	float4 finalColor = saturate((dot((float3) - lightDir, vOut.normal) * 1 / 2 + 0.5f) * lightColor);
	//float4	Color = texDiffuse.Sample(samLinear, vOut.tex) * finalColor;
	
	float4 Color = vOut.color * finalColor;
	Color.a = 1.0f;
	
	return Color;

	//float4	texColor = texDiffuse.Sample(samLinear, vOut.tex) * finalColor;
	//texColor.a = 1.0f;

	//return texColor;
	/*return saturate((dot((float3) -lightDir, vOut.normal) * 1 / 2 + 0.5f) * lightColor);*/
	//return saturate((dot((float3) - lightDir, vOut.normal) * 1 / 10 + 0.5f)) ;
	//return dot((float3) - lightDir, vOut.normal);
}