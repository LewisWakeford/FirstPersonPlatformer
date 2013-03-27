#version 330

in vec3 v3_position;
in vec2 v2_texcoord;
in vec3 v3_normal;

uniform mat4 m_MVP;

smooth out vec2 v2_vertex_texcoord;

void main()
{
    vec4 temp = vec4(v3_position, 1.0);
	gl_Position = m_MVP * temp;
	v2_vertex_texcoord = v2_texcoord;
}
