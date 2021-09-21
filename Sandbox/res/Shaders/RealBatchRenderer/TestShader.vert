#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in int  a_MaterialId;
layout (location = 5) in int  a_TransformId;

out vec4 v_Position;
out vec2 v_UV;
out vec3 v_Normal;
flat out int v_MID;

uniform mat4 u_ProjectionMatrix = mat4(1.0);
uniform mat4 u_ViewMatrix       = mat4(1.0);
uniform samplerBuffer u_TransformTexture;

mat4 GetMatrix(int id)
{
    return mat4(texelFetch(u_TransformTexture, id * 4 + 0),
                texelFetch(u_TransformTexture, id * 4 + 1),
                texelFetch(u_TransformTexture, id * 4 + 2),
                texelFetch(u_TransformTexture, id * 4 + 3));
}

void main()
{
    v_UV = a_TexCoord;
    v_Normal = a_Normal;
    v_MID = a_MaterialId;
    mat4 ModelTransform = GetMatrix(a_TransformId);
    v_Position = ModelTransform * vec4(a_Position, 1.0f);
    vec4 Position = u_ProjectionMatrix * u_ViewMatrix * ModelTransform * vec4(a_Position, 1.0f);
    //vec4 Position = vec4(position, 1.0f);
    gl_Position = Position;
}
