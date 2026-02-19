#version 450 core
out vec4 FragColor;
in vec2 vUV;

uniform sampler2D u_Src;

void main()
{
    vec3 color = texture(u_Src, vUV).rgb;

    // Dither
    float n = fract(sin(dot(gl_FragCoord.xy, vec2(12.9898,78.233))) * 43758.5453);
    color += (n - 0.5) / 255.0;

    FragColor = vec4(color, 1.0);
}
