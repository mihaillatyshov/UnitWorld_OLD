#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in int  a_MaterialId;
layout (location = 2) in int  a_DisplayedFaces;

//flat out int v_MID;


out VS_OUT 
{
    flat int v_MID;
    flat int v_DF;
} vs_to_gs;

void main()
{
    vs_to_gs.v_MID = a_MaterialId;
    vs_to_gs.v_DF = a_DisplayedFaces;

    vec4 Position = vec4(a_Position, 1.0f);
    gl_Position = Position;
}
