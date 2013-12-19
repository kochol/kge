float4x4 matViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;  
};

VS_OUTPUT main( VS_INPUT Input )
{
    VS_OUTPUT Output;
	
    Output.Position = mul( Input.Position, matViewProjection );
    Output.Texcoord = Input.Texcoord;
	
	return( Output );
   
}
