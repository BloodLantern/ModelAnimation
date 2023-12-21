#version 460

layout (location = 0) in vec3 inputPosition;
//layout (location = 1) in vec4 boneIndices;
//layout (location = 2) in vec4 boneWeights;

uniform mat4 modelViewMatrix;

uniform mat4 projectionMatrix;

uniform mat4 skin[64];

void main(void)
{
    /*vec4 posRest = vec4(inputPosition, 1.0f);
    vec4 pos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < 4; i++)
    {
        int idx = int(boneIndices[i]);
        pos += boneWeights[i] * (skin[idx] * posRest);
    }*/
    
    gl_Position = projectionMatrix * (modelViewMatrix * vec4(inputPosition.xyz, 1.0f));
}
