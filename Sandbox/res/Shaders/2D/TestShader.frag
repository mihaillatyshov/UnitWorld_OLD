#version 330 core

layout (location = 0) out vec4 color;

in vec2 uv;
flat in int mid;

//uniform sampler2D uTexture;
uniform sampler2D uTextures[31];
uniform vec4 colors[2];

void main()
{
	color = colors[mid] * texture(uTextures[0], uv);
}
