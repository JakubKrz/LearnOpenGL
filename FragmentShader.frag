#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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

out vec4 FragColor;

void main()
{
    vec3 ambient = material.ambient * light.ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    vec3 viewDir = normalize(-FragPos);
    vec3 halfVector = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfVector, norm), 0.0f), material.shininess);
    vec3 specular = spec * material.specular * light.specular;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}