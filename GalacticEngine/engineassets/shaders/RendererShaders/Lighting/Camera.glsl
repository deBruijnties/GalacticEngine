#ifndef CAMERA_GLSL
#define CAMERA_GLSL


// uniforms for importing camera data in a .h so i can not include if already defined
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ViewProjection;
uniform mat4 u_InverseViewProjection;
uniform vec3 u_CameraPos;


#endif