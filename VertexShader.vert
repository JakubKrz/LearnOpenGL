#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float offset;
uniform float offsetCos;

out vec3 ourColor;
out vec2 texCoord;

void main()
{
   gl_Position = vec4(aPos.x + offset, -aPos.y + offsetCos, aPos.z, 1.0);
   ourColor = gl_Position.xyz;
   texCoord = aTexCoord;
};