#version 330

in vec2 v2_position;

uniform float f_xOffset;
uniform float f_yOffset;
uniform mat4 m_MVP;

void main()
{
	vec4 temp = vec4(v2_position.x + f_xOffset, v2_position.y + f_yOffset, 0.5, 1.0);
	gl_Position = m_MVP * temp;
}