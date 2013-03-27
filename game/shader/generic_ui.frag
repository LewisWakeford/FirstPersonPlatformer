#version 330

uniform vec4 v4_color;

out vec4 v4_frag_color;

void main()
{
	v4_frag_color = v4_color;
}