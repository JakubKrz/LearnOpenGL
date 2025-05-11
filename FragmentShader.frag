#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrenght = 2.0f;
    vec3 viewDir = normalize(-FragPos);
    vec3 halfVector = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfVector, norm), 0.0f), 64);
    vec3 specular = spec * lightColor * specularStrenght;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}