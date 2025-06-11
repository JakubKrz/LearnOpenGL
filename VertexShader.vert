#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    mat4 modelView = view * model;
    FragPos = vec3(modelView * vec4(aPos, 1.0f));
    Normal = mat3(normalMatrix) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * vec4(FragPos, 1.0f);
} 