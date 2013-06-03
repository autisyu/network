////////////////////////////////////////////////////////////////////////////////
// Filename: skydome.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
float4 apexColor;
float4 centerColor;


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 domePosition : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType SkyDomeVertexShader(VertexInputType input)
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Send the unmodified position through to the pixel shader.
	output.domePosition = input.position;

    return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 SkyDomePixelShader(PixelInputType input) : SV_Target
{
	float height;
	float4 outputColor;


	// Determine the position on the sky dome where this pixel is located.
	height = input.domePosition.y;

	// The value ranges from -1.0f to +1.0f so change it to only positive values.
	if(height < 0.0)
	{
		height = 0.0f;
	}

	// Determine the gradient color by interpolating between the apex and center based on the height of the pixel in the sky dome.
	outputColor = lerp(centerColor, apexColor, height);

    return outputColor;
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 SkyDomeTechnique
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_4_0, SkyDomeVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, SkyDomePixelShader()));
        SetGeometryShader(NULL);
    }
}