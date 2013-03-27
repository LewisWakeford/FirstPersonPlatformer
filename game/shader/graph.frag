#version 330

smooth in float height;
smooth in vec4 vertexPos;
smooth in vec4 vertexNormal;
in int in3D;

uniform vec4 fLightVector;

uniform vec4 fDiffuseColor;
uniform vec4 fAmbientColor;

out vec4 fragColor;

void main()
{
    //vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 color = normalize(vec4(height, 1.0-height, 0.0, 1.0));
	float diffuseTerm = clamp(dot(vertexNormal, normalize(fLightVector)), 0.0, 1.0);
	fragColor = (fAmbientColor * color) + (color * fDiffuseColor * diffuseTerm);
}
