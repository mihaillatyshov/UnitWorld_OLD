#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 36) out;

#define BIT(x) (1 << x)

#define BIT_TOP BIT(0)
#define BIT_BOT BIT(1)
#define BIT_LEFT BIT(2)
#define BIT_RIGHT BIT(3)
#define BIT_FRONT BIT(4)
#define BIT_BACK BIT(5)

in VS_OUT 
{
    flat int v_MID;
    flat int v_DF;
} vs_to_gs[]; 

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

flat out int v_MID;
out vec3 v_Normal;

void DrawPrimitive(vec3 v0, vec3 v1, vec3 v2)
{
    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v0, 1.0f));
    EmitVertex();

    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v1, 1.0f));
    EmitVertex();

    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v2, 1.0f));
    EmitVertex();
    
    EndPrimitive(); 
}

void DrawFace(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 normal)
{
    v_Normal = normal;
    DrawPrimitive(v0, v1, v2); 
    DrawPrimitive(v0, v2, v3); 
}

void main()
{
    v_MID = vs_to_gs[0].v_MID;

    const vec3 p0 = vec3(0.0f, 0.0f, 0.0f);
    const vec3 p1 = vec3(1.0f, 0.0f, 0.0f);
    const vec3 p2 = vec3(1.0f, 1.0f, 0.0f);
    const vec3 p3 = vec3(0.0f, 1.0f, 0.0f);
    const vec3 p4 = vec3(0.0f, 0.0f, 1.0f);
    const vec3 p5 = vec3(1.0f, 0.0f, 1.0f);
    const vec3 p6 = vec3(1.0f, 1.0f, 1.0f);
    const vec3 p7 = vec3(0.0f, 1.0f, 1.0f);

    // Top
    if (bool(vs_to_gs[0].v_DF & BIT_TOP))
    {
        DrawFace(p2, p3, p7, p6, vec3(0.0f, 1.0f, 0.0f));
    }

    // Bot
    if (bool(vs_to_gs[0].v_DF & BIT_BOT))
    {
        DrawFace(p0, p1, p5, p4, vec3(0.0f, -1.0f, 0.0f));
    }

    // Left
    if (bool(vs_to_gs[0].v_DF & BIT_LEFT))
    {
        DrawFace(p5, p1, p2, p6, vec3(1.0f, 0.0f, 0.0f)); 
    }

    // Right
    if (bool(vs_to_gs[0].v_DF & BIT_RIGHT))
    {
        DrawFace(p0, p4, p7, p3, vec3(-1.0f, 0.0f, 0.0f)); 
    }

    // Front
    if (bool(vs_to_gs[0].v_DF & BIT_FRONT))
    {
        DrawFace(p1, p0, p3, p2, vec3(0.0f, 0.0f, -1.0f)); 
    }

    // Back
    if (bool(vs_to_gs[0].v_DF & BIT_BACK))
    {
        DrawFace(p4, p5, p6, p7, vec3(0.0f, 0.0f, 1.0f)); 
    }
}