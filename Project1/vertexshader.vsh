#version 430 core

in vec3 normal;
in vec3 position;
in vec2 uv;
in vec3 color;
out vec2 UV;
out vec3 vColor;
uniform vec3 light_pos;
uniform mat4 projection;
uniform mat4 mv;

out VS_OUT
{
   vec3 N;
   vec3 L;
} vs_out;

void main()
{
    // Calculate view-space coordinate
    vec4 P = mv * vec4(position, 1.0);

    // Calculate normal in view-space
    vs_out.N = mat3(mv) * normal;

    // Calculate light vector
    vs_out.L = light_pos - P.xyz;

    // Calculate the clip-space position of each vertex
    gl_Position = projection * P;

    UV = uv;
    vColor = color;
}
