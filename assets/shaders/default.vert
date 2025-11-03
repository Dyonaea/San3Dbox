#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 instancePos;

uniform mat4 camMatrix;
uniform float gap;

void main()
{
   gl_Position = camMatrix * vec4(aPos + instancePos, 1.0);
}