/////////////////////
// INPUT VARIABLES //
/////////////////////
in lowp vec3 inputPosition;
in lowp vec3 normal;
in lowp vec4 boneIndices;
in lowp vec4 boneWeights;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
smooth out vec2 texCoord;
smooth out vec3 outNormal;

uniform SceneMatrices
{
	mat4 projectionMatrix;
} sm;

uniform mat4 modelViewMatrix;

uniform SkinningMatrices
{
	mat4 mat[64];
} skin;



////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	vec4 posRest = vec4(inputPosition, 1.0f);
	vec4 pos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		int idx = int(boneIndices[i]);
		pos += boneWeights[i] * (skin.mat[idx] * posRest);
	}

	// Calculate the position of the vertex against the world, view, and projection matrices.
	// vec4 pos = vec4(pos, 1.0f);

	gl_Position = sm.projectionMatrix * (modelViewMatrix * vec4(pos.xyz, 1.0f));

	outNormal = mat3(modelViewMatrix) * normal;
	outNormal = normalize(outNormal);
}
