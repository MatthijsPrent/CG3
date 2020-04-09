#version 430 core

uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;

in vec3 vColor;

in VS_OUT
{
   vec3 N;
   vec3 L;
} fs_in;

void main()
{
   gl_FragColor = vec4(mat_ambient + vColor, 1.0);
}
