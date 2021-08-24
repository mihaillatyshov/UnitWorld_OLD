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

void DrawPrimitive2(vec2 v1, vec2 v2, vec2 v3)
{
    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v1, 0.0f, 1.0f));
    EmitVertex();

    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v2, 0.0f, 1.0f));
    EmitVertex();

    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v3, 0.0f, 1.0f));
    EmitVertex();
    
    EndPrimitive(); 
}

void DrawFace(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 normal)
{
    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v0, 1.0f));
    EmitVertex();
    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v1, 1.0f));
    EmitVertex();
    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v2, 1.0f));
    EmitVertex();
    EndPrimitive(); 
    
    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v0, 1.0f));
    EmitVertex();
    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v2, 1.0f));
    EmitVertex();
    gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(v3, 1.0f));
    EmitVertex();
    EndPrimitive(); 
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

/*
    if (bool(vs_to_gs[0].v_DF & BIT_TOP))
    {
        v_MID = 0;
        DrawFace(p3, p2, p6, p7, vec3(0.0f, 1.0f, 0.0f));
    }

    if (bool(vs_to_gs[0].v_DF & BIT_BOT))
    {
        v_MID = int(255.0f * 0.2f);
        DrawFace(p1, p0, p4, p5, vec3(0.0f, 1.0f, 0.0f));
    }

    if (bool(vs_to_gs[0].v_DF & BIT_LEFT))
    {
        v_MID = int(255.0f * 0.4f);
        DrawFace(p4, p0, p3, p7, vec3(0.0f, 1.0f, 0.0f));
    }

    if (bool(vs_to_gs[0].v_DF & BIT_RIGHT))
    {
        v_MID = int(255.0f * 0.6f);
        DrawFace(p5, p1, p2, p6, vec3(0.0f, 1.0f, 0.0f));
    }

    if (bool(vs_to_gs[0].v_DF & BIT_FRONT))
    {
        v_MID = int(255.0f * 0.8f);
        DrawFace(p0, p1, p2, p3, vec3(0.0f, 1.0f, 0.0f));
    }

    if (bool(vs_to_gs[0].v_DF & BIT_BACK))
    {
        v_MID = int(255.0f * 1.0f);
        DrawFace(p5, p4, p7, p6, vec3(0.0f, 1.0f, 0.0f));
    }
*/

    // Front
    if (bool(vs_to_gs[0].v_DF & BIT_FRONT))
    {
        v_MID = 0;
        gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(0.0f, 0.0f, 0.0f, 1.0f));
        EmitVertex();
        gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(1.0f, 0.0f, 0.0f, 1.0f));
        EmitVertex();
        gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(1.0f, 1.0f, 0.0f, 1.0f));
        EmitVertex();
        EndPrimitive(); 
    }

    if (bool(vs_to_gs[0].v_DF & BIT_BACK))
    {
        v_MID = 255;
        gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(0.0f, 0.0f, 1.0f, 1.0f));
        EmitVertex();
        gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(1.0f, 0.0f, 1.0f, 1.0f));
        EmitVertex();
        gl_Position = u_ProjMat * u_ViewMat * (gl_in[0].gl_Position + vec4(1.0f, 1.0f, 1.0f, 1.0f));
        EmitVertex();
        EndPrimitive(); 
    }
}