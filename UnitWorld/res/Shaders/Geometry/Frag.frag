#version 330 core

layout (location = 0) out vec4 color;

flat in int v_MID;

void main()
{
	color = vec4(float(v_MID) / 255.0f, 0.2f, 0.8f, 1.0f);
}
