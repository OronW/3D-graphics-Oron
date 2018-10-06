#version 120

in vec2 texcoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

varying  out vec2 Texcoord;

void main(void)
{
	gl_FrontColor = gl_Color;

	gl_Position = projMatrix * viewMatrix * gl_Vertex;
	gl_Position.z /= 1000.0;

	Texcoord = gl_Vertex.xy * 5;
}