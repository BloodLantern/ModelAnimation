#version 460

in vec2 TexCoord;

out vec4 outputColor;

uniform sampler2D textureSampler;

void main(void)
{
    outputColor = texture(textureSampler, TexCoord);
}
