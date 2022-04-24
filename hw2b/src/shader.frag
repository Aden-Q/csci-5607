#version 330 core

layout (location=0) out vec4 out_fragcolor;

in vec3 position;
in vec3 color;
in vec3 normal;

void main(){
    
    // hard code some material properties
    float ka = 0.3f;
    float kd = 0.6f;
    
    // hard code a static point light source at (0,0,0)
    // L is a unit vector from the fragment location towards this light
    vec3 L = -1.f*normalize(vec3(position));

    // normalize the input normal that was interpolated from the mesh vertices
	vec3 N = normalize(normal);
    
    // compute a simple diffuse shading weight that is agnostic to the order in which the triangle vertices were specified
    float N_dot_L = dot(N, L);
	if ((N_dot_L) < 0.0) { N_dot_L *= -1.0; }
    
    // use a simplified ambient+diffuse shading model to define the fragment color
    vec3 result = ka * color + kd * color * N_dot_L;
	out_fragcolor = vec4( result, 1.0 );
} 


