#version 460

layout (location = 0) in vec3 inputPosition;

uniform mat4 modelViewMatrix;

uniform mat4 projectionMatrix;

void main(void)
{
    gl_Position = projectionMatrix * (modelViewMatrix * vec4(inputPosition.xyz, 1.0f));
}
