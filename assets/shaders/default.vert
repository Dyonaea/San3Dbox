#version 330 core

layout (location = 0) in vec3 aPos;
uniform mat4 camMatrix;
uniform float gap;

void main()
{
   gl_Position = camMatrix * vec4(aPos.x + gap, aPos.yz, 1.0);
}