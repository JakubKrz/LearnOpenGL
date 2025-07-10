#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 

struct Light {
    //vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 halfVector = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfVector, norm), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 emissionColor = vec3(texture(material.emission, TexCoords));
    emissionColor.r = emissionColor.g;
    emissionColor.b = emissionColor.g;
    emissionColor *= 0.2;
    if(vec3(texture(material.specular, TexCoords)) != vec3(0.0f, 0.0f, 0.0f))
        {
        emissionColor = vec3(0.0f, 0.0f, 0.0f);
        }
    FragColor = vec4(ambient + diffuse + specular +emissionColor, 1.0);
}