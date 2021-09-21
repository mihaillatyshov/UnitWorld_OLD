#version 330 core

layout (location = 0) out vec4 color;


in VSOutput1
{
    vec2 TexCoord;
    vec3 Normal;
    vec3 WorldPos;
} VSout;


/*
const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];
uniform sampler2D gColorMap;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;


vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, VSOutput1 In)
{
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;
    float DiffuseFactor = dot(In.Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;

        vec3 VertexToEye = normalize(gEyeWorldPos - In.WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, In.Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, gSpecularPower);
        if (SpecularFactor > 0) {
            SpecularColor = vec4(Light.Color, 1.0f) *
                            gMatSpecularIntensity * SpecularFactor;
        }
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcDirectionalLight(VSOutput1 In)
{
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, In);
}

vec4 CalcPointLight(PointLight l, VSOutput1 In)
{
    vec3 LightDirection = In.WorldPos - l.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(l.Base, LightDirection, In);
    float Attenuation =  l.Atten.Constant +
                         l.Atten.Linear * Distance +
                         l.Atten.Exp * Distance * Distance;

    return Color / Attenuation;
}

vec4 CalcSpotLight(SpotLight l, VSOutput1 In)
{
    vec3 LightToPixel = normalize(In.WorldPos - l.Base.Position);
    float SpotFactor = dot(LightToPixel, l.Direction);

    if (SpotFactor > l.Cutoff) {
        vec4 Color = CalcPointLight(l.Base, In);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
    }
    else {
        return vec4(0,0,0,0);
    }
}
*/

uniform vec3 uLight;

uniform float useLight = -10;

void main()
{
/*
		VSOutput1 In;
    In.TexCoord = FSin.TexCoord;
    In.Normal = normalize(FSin.Normal);
    In.WorldPos = FSin.WorldPos;

    vec4 TotalLight = CalcDirectionalLight(In);

    for (int i = 0 ; i < gNumPointLights ; i++) {
        TotalLight += CalcPointLight(gPointLights[i], In);
    }

    for (int i = 0 ; i < gNumSpotLights ; i++) {
        TotalLight += CalcSpotLight(gSpotLights[i], In);
    }
*/
        if (useLight > 0)
        {
    		vec4 Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    		vec3 lightDir = normalize(uLight - VSout.WorldPos);
    		float diff = max(dot(lightDir, VSout.Normal), 0.0);
    		color = diff * Color;
    		color += Color * 0.07f;
    		color.w = 1.0f;
        }
        else
        {
            color = vec4(1.0f);
        }
		//vec4 TotalLight = vec4(0.8f, 0.1f, 0.2f, 1.0f);
    //color = TotalLight;
}
