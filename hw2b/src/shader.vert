#version 330 core

layout(location=0) in vec4 in_position;
layout(location=1) in vec3 in_color;
layout(location=2) in vec3 in_normal;

out vec3 position;
out vec3 color;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // pass the vertex color and normal information to the fragment shader (without any modification)
    color = in_color;
    normal = in_normal;
    
    // determine what the vertex position will be after the model transformation and pass that information to the fragment shader, for use in the illumination calculations
    // in our case the model transformation is the identity matrix so this isn't actually necessary, but it's included here for completeness. Note that the vectors needed for the lighting calculations must be computed using the vertex locations *without* perspective warp applied
    position = vec3(model * in_position);
    
    // apply the model, view, and projection transformations to the vertex position value that will be sent to the clipper, rasterizer, ...
    gl_Position = (projection * view * model * in_position);
}
