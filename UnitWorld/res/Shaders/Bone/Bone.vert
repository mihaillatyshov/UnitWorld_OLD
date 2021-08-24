#version 330 core

layout (location = 0) in vec3  Position;
layout (location = 1) in vec2  TexCoord;
layout (location = 2) in vec3  Normal;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4  Weights;

out VSOutput1
{
    vec2 TexCoord;
    vec3 Normal;
    vec3 WorldPos;
} VSout;

const int MAX_BONES = 100;

uniform mat4 uWVP;
uniform mat4 uWorld;

uniform mat4 uProjection;
uniform mat4 uModel;

uniform mat4 uBones[MAX_BONES];

void main()
{

    mat4 BoneTransform = uBones[BoneIDs[0]] * Weights[0];
    BoneTransform     += uBones[BoneIDs[1]] * Weights[1];
    BoneTransform     += uBones[BoneIDs[2]] * Weights[2];
    BoneTransform     += uBones[BoneIDs[3]] * Weights[3];
  /*
    vec4 PosL      = BoneTransform * vec4(Position, 1.0);
    gl_Position    = gWVP * PosL;
    VSout.TexCoord = TexCoord;
    vec4 NormalL   = BoneTransform * vec4(Normal, 0.0);
    VSout.Normal   = (gWorld * NormalL).xyz;
    VSout.WorldPos = (gWorld * PosL).xyz;
  */
    vec4 PosL      = BoneTransform * vec4(Position, 1.0);
    gl_Position    = uProjection * uModel * PosL;
    VSout.WorldPos = (uModel * PosL).xyz;
    VSout.Normal   = (uModel * vec4(Normal, 0.0f)).xyz;
}
