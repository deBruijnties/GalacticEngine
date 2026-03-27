#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

#include <Camera.glsl>

uniform int u_PointLightCount;

// vec4 packing avoids SSBO padding issues
struct PointLight
{
    vec4 position; // xyz = pos, w = radius
    vec4 color;    // rgb = color, a = intensity
};

// SSBO so we're not UBO light-count limited
layout(std430, binding = 1) buffer PointLights
{
    PointLight lights[];
};

const float PI = 3.14159265359;

vec3 ComputePointLights(vec3 worldPos, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metallic)
{
    vec3 result = vec3(0.0);
    vec3 N = normalize(normal);
    vec3 V = normalize(viewDir);

    roughness = clamp(roughness, 0.04, 1.0); // prevent GGX fireflies at near-zero roughness
    vec3 F0 = mix(vec3(0.04), albedo, metallic); // 0.04 = typical non-metal reflectance

    for (int i = 0; i < u_PointLightCount; i++)
    {
        vec3  Lvec   = lights[i].position.xyz - worldPos;
        float dist   = length(Lvec);
        float radius = lights[i].position.w;

        if (dist > radius) continue; // outside light range, skip

        vec3  L = Lvec / dist;
        vec3  H = normalize(V + L);

        float NdotL = max(dot(N, L), 0.0);
        float NdotV = max(dot(N, V), 0.0);
        float NdotH = max(dot(N, H), 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if (NdotL <= 0.0 || NdotV <= 0.0) continue;

        float attenuation = 1.0 - (dist / radius); // linear falloff, not physical but artist-friendly
        vec3  radiance    = lights[i].color.rgb * lights[i].color.a * attenuation;

        // GGX Normal Distribution
        float a2    = pow(roughness, 4.0); // roughness^2 then ^2 again for perceptual linearity
        float denom = (NdotH * NdotH) * (a2 - 1.0) + 1.0;
        float D     = a2 / (PI * denom * denom);

        // Smith Geometry (Schlick approx, direct lighting remap)
        float k  = pow(roughness + 1.0, 2.0) / 8.0;
        float G  = (NdotV / (NdotV * (1.0 - k) + k))
                 * (NdotL / (NdotL * (1.0 - k) + k));

        // Schlick Fresnel
        vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

        vec3 specular = (D * G * F) / max(4.0 * NdotV * NdotL, 0.001);

        // energy-conserving diffuse - metals have none
        vec3 diffuse = (1.0 - F) * (1.0 - metallic) * albedo / PI;

        result += (diffuse + specular) * radiance * NdotL;
    }

    return result;
}

#endif
