#version 460

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 modelViewMatrix;

uniform mat4 projectionMatrix;

void main(void)
{
    gl_Position = projectionMatrix * (modelViewMatrix * vec4(inputPosition.xyz, 1.0f));
    
    TexCoord = texCoord;
}
