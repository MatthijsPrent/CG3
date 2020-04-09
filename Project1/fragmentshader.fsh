#version 430 core

uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
in vec2 UV;
uniform sampler2D texsampler;

in VS_OUT
{
   vec3 N;
   vec3 L;
} fs_in;

void main()
{
   // Normalize the incoming N and L vectors
   vec3 N = normalize(fs_in.N);
   vec3 L = normalize(fs_in.L);

   // Compute the diffuse component for each fragment
   vec3 diffuse = max(dot(N, L), 0.0) 
                    * texture2D(texsampler, UV).rgb;

   // Write final color to the framebuffer
   gl_FragColor = vec4(mat_ambient + diffuse, 1.0);
}
