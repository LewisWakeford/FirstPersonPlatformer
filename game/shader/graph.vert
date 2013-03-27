#version 330

#define pi 3.141592653589793238462643383279

layout(location = 0) in vec3 vert_pos;

uniform mat4 mat4MVP;
uniform int iIn3D;
uniform float fSampleSpacing;
uniform int iNumSamples;
uniform float fOrigin;
uniform float fAmplitude;
uniform float fOffset;

out float height;
out vec4 vertexPos;
out int in3D;
out vec4 vertexNormal;

void main()
{
    vec4 tempPos = vec4(vert_pos, 1.0);
    vec4 tempNormal = vec4(1.0, 0.0, 0.0, 1.0);
    vertexPos = tempPos;
	if(iIn3D == 0)
	{
	    float d = tempPos.x;
        tempPos.y += sin(d)/d;
	}
	else if(iIn3D == 1)
	{
	    //Normals will probably break with offset, yep they do.
	    float d = sqrt((tempPos.x*tempPos.x) + (tempPos.y*tempPos.y))+fOffset;
	    tempPos.z = (sin(d)/d)*fAmplitude;

	    //Genreate Normal

        //Pretend the graph is 2d by performing operations along the vector from the origin.
        vec4 fromOrigin = normalize(vec4(tempPos.x, tempPos.y, 0.0, 1.0));


        //Find the vector in the imaginary 2d graph with axes A and B instead of X and Y;
        float d1 = d - 0.1;
        float d2 = d + 0.1;

        vec2 sample1 = vec2(d1, (sin(d1)/d1)*fAmplitude);
        vec2 sample2 = vec2(d2, (sin(d2)/d2)*fAmplitude);

        float deltaA = sample2.x - sample1.x;
        float deltaB = sample2.y - sample1.y;

        //2D normal
        vec2 normal2D = vec2(-deltaB, deltaA);

        //Map A onto the vector from the origin to the point.
        vec4 normal4D = fromOrigin*normal2D.x;
        //Replace Z with B.
        normal4D.z = normal2D.y;

        vertexNormal = normalize(normal4D);


	}
	height = (tempPos.z + 1)/2;
	gl_Position = mat4MVP * tempPos;
	in3D = iIn3D;
}
