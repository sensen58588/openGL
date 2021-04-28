#vertexShader begin
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}


#fragmentShader begin
#version 330 core
layout (location = 0) out vec4 color;
void main()
{
   color = vec4(1.0, 0.2, 0.0, 0.0);
}