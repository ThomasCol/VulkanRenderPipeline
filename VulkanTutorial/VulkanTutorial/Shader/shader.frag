#version 450
#extension GL_ARB_separate_shader_objects : enable

struct light
{
	bool enabled;
    vec4 viewPosition;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float attenuation[3];
};

struct material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
};

// Default light
light gDefaultLight[6] = {
light(
	true,
    vec4(1.f, 3.f, 1.f, 0.f),
    vec3(0.5, 0.5, 0.5),
    vec3(0.5, 0.5, 0.5),
    vec3(0.5f, 0.5f, 0.5f),
    float[3](0.1, 0.0, 0.0)),

	light(
	true,
    vec4(-4.71436f,-1.22925f, 2.60048f, 1.f),
    vec3(0.5, 0.5, 0.5),
    vec3(1.0, 180 / 255, 0),
    vec3(1.0, 180 / 255, 0),
    float[3](0.0, 0.0, 2.0)),

	light(
	true,
    vec4(-3.21442f,-1.18025f, 5.54693f, 1.f),
    vec3(0.5, 0.5, 0.5),
    vec3(1.0, 180 / 255, 0),
    vec3(1.0, 180 / 255, 0),
    float[3](0.0, 0.0, 2.0)),

	light(
	true,
    vec4(-2.65886f,-1.18493f, 0.242393f, 1.f),
    vec3(0.5, 0.5, 0.5),
    vec3(1.0, 180 / 255, 0),
    vec3(1.0, 180 / 255, 0),
    float[3](0.0, 0.0, 2.0)),

	light(
	true,
    vec4(0.0180495f, -0.664272f, -2.30163f, 1.f),
    vec3(0.5, 0.5, 0.5),
    vec3(1.0, 180 / 255, 0),
    vec3(1.0, 180 / 255, 0),
    float[3](0.0, 0.0, 2.0)),

	light(
	true,
    vec4(3.02767, -0.658133f, -1.63473f, 1.f),
    vec3(0.5, 0.5, 0.5),
    vec3(1.0, 180 / 255, 0),
    vec3(1.0, 180 / 255, 0),
    float[3](0.0, 0.0, 2.0))
};

// Default material
material gDefaultMaterial = material(
    vec3(0.2, 0.2, 0.2),
    vec3(0.8, 0.8, 0.8),
    vec3(0.0, 0.0, 0.0),
    vec3(0.0, 0.0, 0.0),
    32.f);

// Phong shading function
// viewPosition : fragment position in view-space
//   viewNormal : fragment normal in view-space
vec3 light_shade(light light, material material, vec3 viewPosition, vec3 viewNormal)
{
	if (!light.enabled)
		return vec3(0.0);

    vec3 lightDir;
    float lightAttenuation = 1.0;
    if (light.viewPosition.w > 0.0)
    {
        // Point light
        vec3 lightPosFromVertexPos = (light.viewPosition.xyz / light.viewPosition.w) - viewPosition;
        lightDir = normalize(lightPosFromVertexPos);
        float dist = length(lightPosFromVertexPos);
        lightAttenuation = 1.0 / (light.attenuation[0] + light.attenuation[1]*dist + light.attenuation[2]*light.attenuation[2]*dist);
    }
    else
    {
        // Directional light
        lightDir = normalize(light.viewPosition.xyz);
    }

    if (lightAttenuation < 0.001)
        return vec3(0.0);

    vec3 viewDir  = normalize(-viewPosition);
	vec3 reflectDir = reflect(-lightDir, viewNormal);
	float specAngle = max(dot(reflectDir, viewDir), 0.0);

    vec3 ambient  = lightAttenuation * material.ambient  * light.ambient;
    vec3 diffuse  = lightAttenuation * material.diffuse  * light.diffuse  * max(dot(viewNormal, lightDir), 0.0);
    vec3 specular = lightAttenuation * material.specular * light.specular * (pow(specAngle, material.shininess / 4.0));
	specular = clamp(specular, 0.0, 1.0);
    
	return ambient + diffuse + specular;
}

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 vViewPos;
layout(location = 2) in vec3 vViewNormal;
layout(location = 3) in mat4 vView;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = texture(texSampler, fragTexCoord);

    // Compute phong shading
    vec3 phongColor = gDefaultMaterial.emission;
	for (int i = 0; i < 6; ++i)
    {
        light l = gDefaultLight[i];
        l.viewPosition = vView * gDefaultLight[i].viewPosition;
        phongColor += light_shade(l, gDefaultMaterial, vViewPos, normalize(vViewNormal));
    }
    
    // Apply light color
    outColor.rgb *= phongColor;
}