#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float offset;
uniform float offsetCos;

out vec3 ourColor;
void main()
{
   gl_Position = vec4(aPos.x + offset, -aPos.y + offsetCos, aPos.z, 1.0);
   ourColor = gl_Position.xyz;
};