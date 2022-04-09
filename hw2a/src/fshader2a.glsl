// fragment shader template

#version 150
in vec4 vcolor;
out vec4 color;
void main() 
{
	color = vcolor;  // set output color to interpolated color from vshader
}