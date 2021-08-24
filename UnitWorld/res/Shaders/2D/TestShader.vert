#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in int MaterialId;
layout (location = 3) in int TransformId;

out vec2 uv;
flat out int mid;

uniform mat4 transforms[2];

uniform samplerBuffer tex;

mat4 getMatrix(int id)
{
    return mat4(texelFetch(tex, id * 4 + 0),
                texelFetch(tex, id * 4 + 1),
                texelFetch(tex, id * 4 + 2),
                texelFetch(tex, id * 4 + 3));
}

void main()
{
    uv = texCoord * 4.0f;
    mid = MaterialId;
    mat4 transform = getMatrix(TransformId);

    vec4 Position = transform * vec4(position, 1.0f);

    gl_Position = Position;
}
