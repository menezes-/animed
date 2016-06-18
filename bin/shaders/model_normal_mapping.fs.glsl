#version 330 core

struct PointLight{
    vec3 position;

    float constant;

    float linear;

    float quadratic;

    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

};

struct Material {
    sampler2D texture_ambient1;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    float shininess;
}; 



#define N_POINT_LIGHTS 2

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform PointLight pointLights[N_POINT_LIGHTS];
uniform Material material;

out vec4 color;

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos);

void main()
{
    vec3 normal = Normal;



    normal = texture(material.texture_normal1, TexCoords).rgb;


    normal = normalize(normal * 2.0 - 1.0);


    vec3 result;

    for(int i = 0; i < N_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], material, normal, FragPos);


    color = vec4(result, 1.0);

}


vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos){
    // Diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    float diff = max(dot(lightDir, normal), 0.0);


    // Specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);


    // Attenuation
    float distance = length(light.position - fragPos);

    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


    // Combine os resultados + combina as cores das luzes com as texturas

    vec3 ambient = light.ambient * vec3(texture(mat.texture_ambient1, TexCoords));

    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.texture_diffuse1, TexCoords));

    vec3 specular = light.specular * spec * vec3(texture(mat.texture_specular1, TexCoords));

    ambient *= attenuation;

    diffuse *= attenuation;

    specular *= attenuation;

    return (ambient + diffuse + specular);
}
