float4x4 matViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL;
   float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float    Lit: TEXCOORD1;
   float	Height: TEXCOORD2;
   
};

VS_OUTPUT main( VS_INPUT Input )
{
float3 DirLit = { -0.00078, 0.86189, 0.51568 };
    VS_OUTPUT Output;
	
    Output.Position = mul( Input.Position, matViewProjection );
    Output.Texcoord = Input.Texcoord;
    Output.Lit = 0.5 * dot(DirLit, Input.Normal) + 0.5;
	Output.Height = Input.Position.y / 25.0;
	
	return( Output );
   
}
