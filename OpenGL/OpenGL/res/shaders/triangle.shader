#vertexShader begin
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 0) in vec3 aColor;
uniform vec4 ourColor;
out vec3 outColor;

void main()
{	
	gl_Position = vec4(aPos, 1.0);
	outColor = vec3(aColor.xyz * ourColor.xyz);
}


#fragmentShader begin
#version 330 core

in vec3 outColor;
out vec4 color;
void main()
{
   color = vec4(outColor, 1.0);
}