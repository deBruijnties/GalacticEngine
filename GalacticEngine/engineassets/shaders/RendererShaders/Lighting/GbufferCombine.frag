#version 450 core

out vec4 FragColor;
in  vec2 vUV;

// G-buffer inputs - each texture packs two pieces of data into rgba to save samplers
// albedo(rgb) + metallic(a)
uniform sampler2D gAlbedoMetal;
// world-space normal(rgb) + roughness(a)
uniform sampler2D gNormalRough;
// emission(rgb) + ambient occlusion(a)
uniform sampler2D gEmissionAO;
// standard hardware depth buffer from the geometry pass
uniform sampler2D gDepth;

#include <Camera.glsl>   // gives us u_CameraPos, u_InverseViewProjection
#include <Lighting.glsl> // gives us ComputePointLights()

// turns a screen-space UV + depth sample back into a world-space position
// this lets us run the lighting pass without storing world pos as a separate G-buffer texture
vec3 ReconstructWorldPosition(vec2 uv, float depth)
{
    // go from [0,1] UV and depth into [-1,1] NDC space
    vec4 ndc = vec4(
        uv * 2.0 - 1.0,
        depth * 2.0 - 1.0,
        1.0
    );
    // unproject back to world space using the inverse view-projection matrix
    vec4 world = u_InverseViewProjection * ndc;
    return world.xyz / world.w; // perspective divide
}

void main()
{
    float depth = texture(gDepth, vUV).r;

    // depth of exactly 1.0 means nothing was written here during the geometry pass - it's sky
    // cheaper to discard here than to let it go through the full lighting calculation
    if (depth >= 1.0)
        discard;

    // reconstruct where in the world this pixel lives
    vec3 worldPos = ReconstructWorldPosition(vUV, depth);

    // unpack all the G-buffer data for this pixel
    vec4 albedoMetal = texture(gAlbedoMetal, vUV);
    vec4 normalRough = texture(gNormalRough, vUV);
    vec4 emissionAO  = texture(gEmissionAO,  vUV);

    vec3  albedo    = albedoMetal.rgb;
    float metallic  = albedoMetal.a;

    // normals are stored in [0,1] range, remap back to [-1,1] and re-normalize
    // (interpolation across the fullscreen quad can denormalize them slightly)
    vec3  normal    = normalize(normalRough.rgb * 2.0 - 1.0);
    float roughness = normalRough.a;

    vec3  emission  = emissionAO.rgb;
    float ao        = emissionAO.a;

    // direction from the surface toward the camera, used for specular calculations
    vec3 viewDir = normalize(u_CameraPos - worldPos);

    // run PBR lighting for all point lights in the scene
    vec3 lighting = ComputePointLights(
        worldPos,
        normal,
        viewDir,
        albedo,
        roughness,
        metallic
    );

    // tiny constant ambient term so nothing ever goes completely black
    // multiplied by AO so occluded crevices still get darkened
    lighting += albedo * 0.03 * ao;

    // emissive surfaces just add straight to the result, no lighting calculation needed
    lighting += emission;

    FragColor = vec4(lighting, 1.0);
}
