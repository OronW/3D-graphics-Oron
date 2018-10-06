#version 130

in vec2 Texcoord;
uniform sampler2D tex;

void main(void)
{
	
	gl_FragColor = texture(tex, Texcoord);
}