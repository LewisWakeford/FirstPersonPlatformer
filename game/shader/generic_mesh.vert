#version 330

in vec3 v3_position;
in vec2 v2_texcoord;
in vec3 v3_normal;

uniform mat4 m_MVP;
uniform mat3 m_normal;

smooth out vec2 v2_vertex_texcoord;
smooth out vec3 v3_vertex_normal;

void main()
{
    vec4 temp = vec4(v3_position, 1.0);
    v3_vertex_normal = m_normal * v3_normal;
    v2_vertex_texcoord = v2_texcoord;

    gl_Position = m_MVP * temp;	
}
