#version 330 core

layout (location = 0) out vec4 color;

#define AMBI 0.5f

flat in int v_MID;
in vec3 v_Normal;

void main()
{
	vec3 lightDir = normalize(vec3(0.5f, 1.0f, 0.5f));
    float diff = max(dot(lightDir, v_Normal), 0.0f);
 	
	vec3 ambient  = 0.5f * AMBI * vec3(float(v_MID) / 255.0f, 0.2f, 0.8f);
    vec3 diffuse  = 1.0f * diff * vec3(float(v_MID) / 255.0f, 0.2f, 0.8f);
    
	color = vec4(ambient + diffuse, 1.0f);
	//color = vec4(float(v_MID) / 255.0f, 0.2f, 0.8f, 1.0f);
}
