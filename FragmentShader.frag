#version 330 core
struct Material {
    sampler2D diffuse;
    vec3      specular;
    float     shininess;
}; 

struct Light {
    vec3 position;
  
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
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 halfVector = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfVector, norm), 0.0f), material.shininess);
    vec3 specular = spec * material.specular * light.specular;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}