#version 330 core
out vec4 FragColor;
 
in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform float range;

void main()
{
    FragColor = mix(texture(myTexture1, texCoord), texture(myTexture2, texCoord), abs(0.5f));
    //texture(myTexture1, texCoord) * vec4(ourColor +0.5, 1.0);
} 