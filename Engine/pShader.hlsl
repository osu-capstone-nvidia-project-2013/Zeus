cbuffer Lighting : register(cb0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularcolor;
	float specularPower;
	float3 lightDirection;
}

cbuffer Mapping : register(cb1)
{
	float textureflag;
	float alphaflag;
	float normalflag;
	float particleflag;
	float reflectflag;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float3 normal : NORMAL;
	float3 viewDirection : VIEWDIRECTION;
	float2 texcord : TEXCORD;
	float3x3 tbnmatrix : TBNMATRIX;
	float3 reflect : REFLECT;
};

Texture2D Texture;
Texture2D Alpha;
Texture2D NormalMap;
texture2D ReflectMap;
SamplerState ss;

float4 PShader(VOut input) : SV_TARGET
{
	float4 textureColor;
    float4 alphaColor;
	float4 normalColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;
	float resAlpha;

	if(reflectflag == 1.)
	{
		
	}
	

	//////////////////////////check if pixel kill needed///////////////////////////////////
	if(alphaflag == 1.)
	{
		alphaColor = Alpha.Sample(ss, input.texcord);

		clip(((alphaColor.r + alphaColor.g + alphaColor.b ) / 3.0f) < 0.1f ? -1:1);
		
	}
	else
	{
		clip(input.color.a < 0.1f ? -1:1);
	}
	////////////////////////////////////////////////////////////////////////////////////////


	
	//////////////////////////////////grab texture color////////////////////////////////////
	if(textureflag == 1.)
	{
		textureColor = Texture.Sample(ss, input.texcord);
		if(alphaflag == 1.0)
		{
			textureColor.a = (alphaColor.r + alphaColor.g + alphaColor.b ) / 3.0;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////

	

	////////////////////////////////////normal mapping////////////////////////////////////

	if(normalflag == 1.)
	{
		normalColor = NormalMap.Sample(ss, input.texcord);
		normalColor = 2.0f*normalColor-1.0f;
		input.normal = normalize( float3(normalColor.x,normalColor.y,normalColor.z));
		input.normal = mul(input.normal, transpose(input.tbnmatrix));
	}

	//////////////////////////////////////////////////////////////////////////////////////
    

	//////////////////////////lighting prep///////////////////////////////////////////////
	
	color = ambientColor;

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

	lightDir = normalize(lightDir);
    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));
  
	//Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	///////////////////////////////////////////////////////////////////////////////////////


	/////////////////////apply diffuse and calculate specular lighting///////////////////////////////
    if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);

        // Saturate the ambient and diffuse color.
        color = saturate(color);
		if(particleflag == 0)
		{
			// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
			reflection = normalize(2 * lightIntensity * input.normal - lightDir); 
		
			// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
			specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
		}
    }

	////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////apply texture/native color///////////////////////////////////
	if(textureflag != 1.)
	{
		color = color * input.color;
	}
	else
	{
		color = color * textureColor;
	}
	////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////apply specular lighting////////////////////////////////////////////
		//particles don't get specular lighting
	if(particleflag == 0)
	{  
		// Add the specular component last to the output color.
		color = saturate(color + specular);
	}
	////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////alpha mapping//////////////////////////////////////////////////////
	if(alphaflag == 1.)
	{
		color.a = (alphaColor.r + alphaColor.g + alphaColor.b ) / 3.0;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	

    return color;
}
